/*----------------------------------------------------------------------------
  Motel Input Output

  application programmer's interface (API) header file
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

#ifndef MOTEL_IO_I_H
#define MOTEL_IO_I_H

#include "motel.compilation.t.h"
#include "motel.types.t.h"

#include "motel.io.t.h"

/*----------------------------------------------------------------------------
  WriteMotelDocument()
  ----------------------------------------------------------------------------
  Serialize a MOTEL document into pDocument: one ":<Name>:<Value>" declaration
  per header field, followed by a single streaming member ":<PayloadName>,*:"
  whose value is pPayload and which runs to the end of the document. The
  streaming member carries an arbitrary opaque payload (for example a JSON
  state image) without escaping. Header values must be single-line.
  ----------------------------------------------------------------------------
  Parameters:

  pDocument     - (O) Buffer to receive the serialized document
  pDocumentSize - (I) Size of pDocument (including the string terminator)
  pFields       - (I) Array of header declarations
  pFieldCount   - (I) Number of header declarations
  pPayloadName  - (I) Object name of the streaming member
  pPayload      - (I) Opaque streaming-member payload
  ----------------------------------------------------------------------------
  Returns:

  TRUE  - the document was serialized within pDocumentSize
  FALSE - bad argument, or the document did not fit within pDocumentSize
  ---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION WriteMotelDocument
(
    char * pDocument,
    size_t pDocumentSize,
    const motelField * pFields,
    size_t pFieldCount,
    const char * pPayloadName,
    const char * pPayload
);

/*----------------------------------------------------------------------------
  ReadMotelDocument()
  ----------------------------------------------------------------------------
  Parse a MOTEL document produced by WriteMotelDocument(). Header declarations
  are copied into pFields; the streaming member's name and payload are returned
  as pointers into pDocument (zero-copy). pDocument is modified in place (header
  names and values are terminated within it). The payload runs to the end of
  the document.
  ----------------------------------------------------------------------------
  Parameters:

  pDocument      - (IO) The document to parse (modified in place)
  pFields        - (O)  Array to receive header declarations
  pFieldCapacity - (I)  Capacity of pFields
  pFieldCount    - (O)  Number of header declarations parsed
  pPayloadName   - (O)  Receives a pointer (into pDocument) to the streaming member name
  pPayload       - (O)  Receives a pointer (into pDocument) to the streaming member payload
  ----------------------------------------------------------------------------
  Returns:

  TRUE  - the document was well formed and a streaming member was found
  FALSE - bad argument, malformed declaration, or pFieldCapacity exceeded
  ---------------------------------------------------------------------------*/

EXPORT_STORAGE_CLASS success CALLING_CONVENTION ReadMotelDocument
(
    char * pDocument,
    motelField * pFields,
    size_t pFieldCapacity,
    size_t * pFieldCount,
    const char ** pPayloadName,
    const char ** pPayload
);

#endif
