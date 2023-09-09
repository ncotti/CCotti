#include "sem.h"

/// @brief Creates a semaphore with an initial "semval" value of 1,
///  or connects to an existing one. See "Sem::op" to know how to operate it.
/// @param path Path to any file. Necessary to identify the semaphore.
/// @param id Can be any value. Identifies the semaphore.
/// @param create If "true", the semaphore will be created. If "false", it will
///  try to connect to an already existing one, with the same "path" and "id".
/// @return Throws "std::runtime_error" in case of error.
Sem::Sem(const char* path, int id, bool create): creator(create) {
    key_t key;
    if ( (key = ftok(path, id) ) == -1) {
        perror(ERROR("Couldn't get semaphore with ftok.\n"));
        throw(std::runtime_error("Ftok"));
    }

    if (create) {
        if ( (this->semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666) ) == -1 ) {
            perror(ERROR("Couldn't create semaphore with semget.\n"));
            throw(std::runtime_error("Create"));
        }
        if (this->set(1) != 0) {
            perror(ERROR("Couldn't set semaphore value.\n"));
            throw(std::runtime_error("Set_value"));
        }
    } else {
        if ( (this->semid = semget(key, 0, 0) ) == -1 ) {
            perror(ERROR("Couldn't connect to existing semaphore.\n"));
            throw(std::runtime_error("Connect"));
        }
    }
}

/// @brief  Destroys the semaphore, free resources. Only the creator will be
///  be able to remove it.
Sem::~Sem(void) {
    if (this->creator) {
        semctl(this->semid, 0, IPC_RMID);
    }
}

/// @brief Checks if the semaphore already exists.
/// @param path Path to any file. Necessary to identify the semaphore.
/// @param id Can be any value. Identifies the semaphore.
/// @return "true" if it already exists, "false" otherwise.
bool Sem::exists(const char* path, int id) {
    key_t key;
    if ( (key = ftok(path, id) ) == -1) {
        return false;
    }
    if ( (semget(key, 0, 0) ) == -1 ) {
        return false;
    }
    return true;
}

/// @brief Sets the semaphore's "semval" to a specific value.
/// @return "0" on success, "-1" on error.
char Sem::set (int value) {
    return semctl(this->semid, 0, SETVAL, value);
}

/// @brief Returns the value of the semaphore.
char Sem::get(void) const {
    return semctl(this->semid, 0, GETVAL);
}

/// @brief Realizes one of the following operations, depending on the argument "op":
///  op = 0;  The proccess blocks until the value of the semaphore equals "0".
///  op > 0;  Adds that value to the semaphore "semval".
///  op < 0;  If |op| <= |semval|, then semval = semval - op
///           If |op| >  |semval|, then the operation blocks until "op" can be
///           subtracted from "semval", with the end result a positive number or "0".
/// @param op Operation value.
/// @return "0" on success, "-1" on error.
char Sem::op (int op) {
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = op;
    sop.sem_flg = 0;
    return semop(this->semid, &sop, 1);
}

/******************************************************************************
 * Overloaded operators
******************************************************************************/

/// @brief Adds "+1" to the semaphore's "semval".
int Sem::operator++ (int) {
    this->op(1);
    return this->get();
}

int Sem::operator++ () {
    this->op(1);
    return this->get();
}

/// @brief Subtracts "-1" to the semaphore's "semval".
int Sem::operator-- (int) {
    this->op(-1);
    return this->get();
}

int Sem::operator-- () {
    this->op(-1);
    return this->get();
}

/// @brief Sets the semaphore's value
int Sem::operator= (int a) {
    this->set(a);
    return this->get();
}

/// @brief Adds and arbitrary amount to the semaphore. Doesn't update the value.
int Sem::operator+ (int a) {
    return this->get() + a;
}

/// @brief Subtracts and arbitrary amount to the semaphore. Doesn't update the value.
int Sem::operator- (int a) {
    return this->get() - a;
}
