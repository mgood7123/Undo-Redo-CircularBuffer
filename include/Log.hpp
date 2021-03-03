//
// Created by Matthew Good on 2/3/21.
//

#ifndef CIRCULARBUFFER_LOG_HPP
#define CIRCULARBUFFER_LOG_HPP

#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/DebugStl.h>

#define LOG_MAGNUM_FLAGS Corrade::Utility::Debug::Flags::Type::NoSpace | Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd

#ifdef __ANDROID__
extern std::ostream streamDebug;
#define LOG_MAGNUM_DEBUG Corrade::Utility::Debug{&streamDebug, LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Green)
extern std::ostream streamWarning;
#define LOG_MAGNUM_WARNING Corrade::Utility::Warning{&streamWarning, LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Yellow)
extern std::ostream streamError;
#define LOG_MAGNUM_ERROR Corrade::Utility::Error{&streamError, LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Red)
extern std::ostream streamFatal;
#define LOG_MAGNUM_FATAL Corrade::Utility::Fatal{&streamFatal, -1, LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Red)
#else
#define LOG_MAGNUM_DEBUG Corrade::Utility::Debug{LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Green)
#define LOG_MAGNUM_WARNING Corrade::Utility::Warning{LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Yellow)
#define LOG_MAGNUM_ERROR Corrade::Utility::Error{LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Red)
#define LOG_MAGNUM_FATAL Corrade::Utility::Fatal{-1, LOG_MAGNUM_FLAGS} << Corrade::Utility::Debug::color(Corrade::Utility::Debug::Color::Red)
#endif

#define LOG_MAGNUM_INFO LOG_MAGNUM_DEBUG

#define LOG_MAGNUM_DEBUG_FUNCTION(x) LOG_MAGNUM_DEBUG << #x << " = " << (x)
#define LOG_MAGNUM_INFO_FUNCTION(x) LOG_MAGNUM_DEBUG_FUNCTION(x)
#define LOG_MAGNUM_WARNING_FUNCTION(x) LOG_MAGNUM_WARNING << #x << " = " << (x)
#define LOG_MAGNUM_ERROR_FUNCTION(x) LOG_MAGNUM_ERROR << #x << " = " << (x)
#define LOG_MAGNUM_FATAL_FUNCTION(x) LOG_MAGNUM_FATAL << #x << " = " << (x)

#endif //CIRCULARBUFFER_LOG_HPP
