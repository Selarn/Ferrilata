#include "responces.h"

// Responces __________

char Responce::get_type() {
    return type;
}

bool Responce::is_error() {
    return error;
}

Responce::Responce(char type ) {
    this->type = type;
    this->error = false;
}

Responce::~Responce() { }

// Errors __________

const QString &ErrorResponce::get_title() {
    return title;
}

const QString &ErrorResponce::get_text() {
    return text;
}

ErrorResponce::ErrorResponce(char type): Responce(type) {
    error = true;
    if ( type == ErrorResponce::UNKNOWN ) {
        title = "error";
        text = "Undescripted error ???";
    }
}

ErrorResponce::~ErrorResponce() { }

TimeLimitErr::TimeLimitErr(const QString &error_text): ErrorResponce( ErrorResponce::TIME_LIMIT ) {
    title = "Waiting time exceeded";
    text = error_text;
}

TimeLimitErr::~TimeLimitErr() { }

PermissionDeniedErr::PermissionDeniedErr(const QString &error_text) {
    title = "Tebe tam ne radi";
    text = error_text;
}

PermissionDeniedErr::~PermissionDeniedErr() { }
