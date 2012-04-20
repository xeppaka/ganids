#include "db.h"

Db::Db():
    db(NULL)
{
}

Db::~Db()
{
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

bool Db::open(const char *filename)
{
    int rc;
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc)
        return false;

    char *zErrMsg = NULL;
    rc = sqlite3_exec(db, "create table if not exists nids_log (timestamp TEXT, message TEXT, rule TEXT, payload BLOB)", NULL, 0, &zErrMsg);
    if (rc) {
        close();
        return false;
    }
    insert("message1", "rule1", "payload1", 8);
    close();

    return true;
}

void Db::close()
{
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

bool Db::insert(const char *message, const char *rule, const char *payload, int payload_size)
{
    if (!db)
        return false;

    sqlite3_stmt *stmt = NULL;
    int rc = 0;
    try {
        rc = sqlite3_prepare_v2(db, "insert into nids_log values (datetime('now'), ?, ?, ?)", -1, &stmt, NULL);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_text(stmt, 1, message, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_text(stmt, 2, rule, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_blob(stmt, 3, payload, payload_size, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
            throw 1;
    } catch (int i)
    {
        if (stmt)
            sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
