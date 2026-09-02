#include <sqlite3.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

// RAII封装SQLite数据库
class SQLiteDB {
private:
    sqlite3* db;
    
public:
    SQLiteDB(const std::string& filename) : db(nullptr) {
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::string error = sqlite3_errmsg(db);
            sqlite3_close(db);
            throw std::runtime_error("Cannot open database: " + error);
        }
        std::cout << "✅ Database opened successfully" << std::endl;
    }
    
    ~SQLiteDB() {
        if (db) {
            sqlite3_close(db);
            std::cout << "✅ Database closed" << std::endl;
        }
    }
    
    // 禁止拷贝
    SQLiteDB(const SQLiteDB&) = delete;
    SQLiteDB& operator=(const SQLiteDB&) = delete;
    
    // 允许移动
    SQLiteDB(SQLiteDB&& other) noexcept : db(other.db) {
        other.db = nullptr;
    }
    
    SQLiteDB& operator=(SQLiteDB&& other) noexcept {
        if (this != &other) {
            if (db) sqlite3_close(db);
            db = other.db;
            other.db = nullptr;
        }
        return *this;
    }
    
    void execute(const std::string& sql) {
        char* errmsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK) {
            std::string error = errmsg;
            sqlite3_free(errmsg);
            throw std::runtime_error("SQL error: " + error);
        }
        std::cout << "✅ SQL executed: " << sql << std::endl;
    }
    
    // 查询回调函数
    static int selectCallback(void* data, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << "------------------------" << std::endl;
        return 0;
    }
    
    void query(const std::string& sql) {
        char* errmsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), selectCallback, nullptr, &errmsg);
        if (rc != SQLITE_OK) {
            std::string error = errmsg;
            sqlite3_free(errmsg);
            throw std::runtime_error("Query error: " + error);
        }
    }
    
    std::string getVersion() {
        const char* version = sqlite3_libversion();
        return std::string(version);
    }
};

int main() {
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "🚀 SQLite3 Application Starting..." << std::endl;
        std::cout << "SQLite3 version: " << sqlite3_libversion() << std::endl;
        std::cout << "========================================" << std::endl << std::endl;
        
        // 创建数据库
        SQLiteDB db("test.db");
        
        // 显示SQLite版本
        std::cout << "📦 SQLite3 library version: " << db.getVersion() << std::endl << std::endl;
        
        // 创建表
        std::cout << "📝 Creating table..." << std::endl;
        db.execute(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "age INTEGER, "
            "email TEXT UNIQUE"
            ");"
        );
        
        // 插入数据
        std::cout << std::endl << "📝 Inserting data..." << std::endl;
        db.execute("INSERT INTO users (name, age, email) VALUES ('Alice', 30, 'alice@example.com');");
        db.execute("INSERT INTO users (name, age, email) VALUES ('Bob', 25, 'bob@example.com');");
        db.execute("INSERT INTO users (name, age, email) VALUES ('Charlie', 35, 'charlie@example.com');");
        
        // 查询数据
        std::cout << std::endl << "📊 Querying data..." << std::endl;
        db.query("SELECT * FROM users;");
        
        // 执行聚合查询
        std::cout << "📊 Average age:" << std::endl;
        db.query("SELECT AVG(age) as average_age FROM users;");
        
        std::cout << std::endl << "✅ Application completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
