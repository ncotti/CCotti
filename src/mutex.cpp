#include "mutex.h"

/// @brief Creates a new Mutex.
Mutex::Mutex() {
    this->mutex = PTHREAD_MUTEX_INITIALIZER;
}

/// @brief Destroys the Mutex, frees resources.
Mutex::~Mutex(void) {
    pthread_mutex_destroy(&(this->mutex));
}

/// @brief Reserves the Mutex in a blocking manner.
/// @return "0" on success, -1 on error.
char Mutex::lock(void) {
    if (pthread_mutex_lock(&(this->mutex)) != 0) {
        perror(ERROR("Failed pthread_mutex_lock.\n"));
        return -1;
    }
    return 0;
}

/// @brief Frees a Mutex variable.
/// @return "0" on success, -1 on error.
char Mutex::unlock(void) {
    if (pthread_mutex_unlock(&(this->mutex)) != 0) {
        perror(ERROR("Failed pthread_mutex_unlock.\n"));
        return -1;
    }
    return 0;
}

/// @brief Tries to reserve the Mutex in a non-blocking manner.
/// @return "0" on success, "-1" on error, or if the mutex was already reserved.
char Mutex::trylock(void) {
    if (pthread_mutex_trylock(&(this->mutex)) != 0) {
        return -1;
    }
    return 0;
}
