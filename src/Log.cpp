//
// Created by Matthew Good on 2/3/21.
//

#include <Log.hpp>

#include <Corrade/Utility/AndroidDesktopLogStreamBuffer.h>

Corrade::Utility::AndroidDesktopLogStreamBuffer bufferDebug {Corrade::Utility::AndroidDesktopLogStreamBuffer::LogPriority::Debug, "GLIS-MAGNUM", true};
std::ostream streamDebug {&bufferDebug};

Corrade::Utility::AndroidDesktopLogStreamBuffer bufferWarning {Corrade::Utility::AndroidDesktopLogStreamBuffer::LogPriority::Warning, "GLIS-MAGNUM", true};
std::ostream streamWarning {&bufferWarning};

Corrade::Utility::AndroidDesktopLogStreamBuffer bufferError {Corrade::Utility::AndroidDesktopLogStreamBuffer::LogPriority::Error, "GLIS-MAGNUM", true};
std::ostream streamError {&bufferError};

Corrade::Utility::AndroidDesktopLogStreamBuffer bufferFatal {Corrade::Utility::AndroidDesktopLogStreamBuffer::LogPriority::Fatal, "GLIS-MAGNUM", true};
std::ostream streamFatal {&bufferFatal};
