/*----------------------------------------------------------------------------
  Motel Input Output

  application programmer's interface (API) implementation file
  ----------------------------------------------------------------------------
  Copyright 2011-2015, 2026 John L. Hart IV. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain all copyright notices,
     this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce all copyright
     notices, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY John L. Hart IV "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
  NO EVENT SHALL John L. Hart IV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

  The views and conclusions contained in the software and documentation are
  those of the authors and should not be interpreted as representing official
  policies, either expressed or implied, of John L Hart IV.
  ----------------------------------------------------------------------------*/

#include "motel.io.h"

/*----------------------------------------------------------------------------
  Embedded copyright
  ----------------------------------------------------------------------------*/

static const char *gCopyright UNUSED = "@(#)motel.io.c - Copyright 2011-2015, 2026 John L. Hart IV - All rights reserved";

/*----------------------------------------------------------------------------
  Private defines
  ----------------------------------------------------------------------------*/

/* MOTEL default reserved graphemes (see motel.document specification) */

#define DECLARATION_DELIMITER ':'
#define LIST_DELIMITER        ','
#define STREAMING_INDICATOR   '*'
#define LINE_DELIMITER        '\n'

/*----------------------------------------------------------------------------
  Public functions
  ----------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION WriteMotelDocument
(
    char * pDocument,
    size_t pDocumentSize,
    const motelField * pFields,
    size_t pFieldCount,
    const char * pPayloadName,
    const char * pPayload
)
{
    size_t lField;
    size_t lRequired;

    if (NULL == pDocument || 0 == pDocumentSize || NULL == pPayloadName || NULL == pPayload)
    {
        return (FALSE);
    }

    /*
    ** compute the required length and refuse if it would overrun the document buffer
    */

    lRequired = 0;

    for (lField = 0; lField < pFieldCount; lField++)
    {
        /* ":" + Name + ":" + Value + "\n" */

        lRequired += 1 + strlen(pFields[lField].Name) + 1 + strlen(pFields[lField].Value) + 1;
    }

    /* ":" + PayloadName + ",*:" + Payload */

    lRequired += 1 + strlen(pPayloadName) + 3 + strlen(pPayload);

    if (lRequired >= pDocumentSize)
    {
        return (FALSE);
    }

    /*
    ** serialize the header declarations (one ":<Name>:<Value>" per line)
    */

    *pDocument = *CC_STRING_TERMINATOR;

    for (lField = 0; lField < pFieldCount; lField++)
    {
        AppendString(":", pDocument, pDocumentSize);
        AppendString(pFields[lField].Name, pDocument, pDocumentSize);
        AppendString(":", pDocument, pDocumentSize);
        AppendString(pFields[lField].Value, pDocument, pDocumentSize);
        AppendString("\n", pDocument, pDocumentSize);
    }

    /*
    ** serialize the streaming member - it is the last member and runs to the
    ** end of the document, so the opaque payload needs no escaping
    */

    AppendString(":", pDocument, pDocumentSize);
    AppendString(pPayloadName, pDocument, pDocumentSize);
    AppendString(",*:", pDocument, pDocumentSize);
    AppendString(pPayload, pDocument, pDocumentSize);

    return (TRUE);
}

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ReadMotelDocument
(
    char * pDocument,
    motelField * pFields,
    size_t pFieldCapacity,
    size_t * pFieldCount,
    const char ** pPayloadName,
    const char ** pPayload
)
{
    char * lCursor;
    char * lName;
    char * lValue;

    if (NULL == pDocument || NULL == pFields || NULL == pFieldCount || NULL == pPayloadName || NULL == pPayload)
    {
        return (FALSE);
    }

    *pFieldCount = 0;
    *pPayloadName = (const char *) NULL;
    *pPayload = (const char *) NULL;

    lCursor = pDocument;

    loop
    {
        /* a well formed document ends with a streaming member, not the terminator */

        if (*CC_STRING_TERMINATOR == *lCursor)
        {
            return (FALSE);
        }

        /* every declaration begins with the declaration delimiter */

        if (DECLARATION_DELIMITER != *lCursor)
        {
            return (FALSE);
        }

        lCursor++;

        /* read the object name up to the value delimiter or the list delimiter */

        lName = lCursor;

        while (DECLARATION_DELIMITER != *lCursor && LIST_DELIMITER != *lCursor && *CC_STRING_TERMINATOR != *lCursor)
        {
            lCursor++;
        }

        if (LIST_DELIMITER == *lCursor)
        {
            /* the streaming member is "<Name>,*:<payload to end of document>" */

            if (STREAMING_INDICATOR != *(lCursor + 1) || DECLARATION_DELIMITER != *(lCursor + 2))
            {
                return (FALSE);
            }

            *lCursor = *CC_STRING_TERMINATOR;

            *pPayloadName = lName;
            *pPayload = lCursor + 3;

            return (TRUE);
        }

        if (DECLARATION_DELIMITER != *lCursor)
        {
            return (FALSE);
        }

        /* a header declaration is "<Name>:<Value>" terminated by end of line */

        *lCursor = *CC_STRING_TERMINATOR;

        lCursor++;

        lValue = lCursor;

        while (LINE_DELIMITER != *lCursor && *CC_STRING_TERMINATOR != *lCursor)
        {
            lCursor++;
        }

        if (*pFieldCount >= pFieldCapacity)
        {
            return (FALSE);
        }

        if (LINE_DELIMITER == *lCursor)
        {
            *lCursor = *CC_STRING_TERMINATOR;

            lCursor++;
        }

        CopyString(lName, pFields[*pFieldCount].Name, MOTEL_FIELD_SIZE);
        CopyString(lValue, pFields[*pFieldCount].Value, MOTEL_FIELD_SIZE);

        (*pFieldCount)++;
    }
}
