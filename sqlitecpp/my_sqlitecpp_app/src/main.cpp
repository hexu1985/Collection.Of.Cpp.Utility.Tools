#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// 简单的用户结构体
struct User {
    int         id;
    std::string name;
    int         age;
    std::string email;
};

int main() {
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "🚀 SQLiteCpp Application Starting..." << std::endl;
        std::cout << "SQLite version: " << SQLite::VERSION << std::endl;
        std::cout << "SQLiteCpp version: " << SQLite::getLibVersion() << std::endl;
        std::cout << "========================================" << std::endl << std::endl;

        // 1. 打开数据库（文件不存在会自动创建）
        SQLite::Database db("test_sqlitecpp.db",
                            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        std::cout << "✅ Database opened: " << db.getFilename() << std::endl;

        // 2. 创建表
        db.exec("DROP TABLE IF EXISTS users");
        db.exec("CREATE TABLE users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL, "
                "age INTEGER, "
                "email TEXT UNIQUE"
                ")");
        std::cout << "✅ Table 'users' created" << std::endl;

        // 3. 使用事务 + 预处理语句批量插入
        {
            SQLite::Transaction transaction(db);

            SQLite::Statement insert(db,
                "INSERT INTO users (name, age, email) VALUES (?, ?, ?)");

            std::vector<User> users = {
                {0, "Alice",   30, "alice@example.com"},
                {0, "Bob",     25, "bob@example.com"},
                {0, "Charlie", 35, "charlie@example.com"},
            };

            for (const auto& u : users) {
                insert.bind(1, u.name);
                insert.bind(2, u.age);
                insert.bind(3, u.email);
                insert.exec();
                insert.reset();
            }

            transaction.commit();
            std::cout << "✅ Inserted " << users.size() << " users" << std::endl;
        }

        // 4. 查询并打印
        std::cout << std::endl << "📊 Querying all users:" << std::endl;
        SQLite::Statement query(db, "SELECT id, name, age, email FROM users");
        while (query.executeStep()) {
            std::cout << "  id="    << query.getColumn(0).getInt()
                      << ", name="  << query.getColumn(1).getText()
                      << ", age="   << query.getColumn(2).getInt()
                      << ", email=" << query.getColumn(3).getText()
                      << std::endl;
        }

        // 5. 使用 VariadicBind 简化绑定（SQLiteCpp 提供）
        std::cout << std::endl << "📊 Query users older than 28:" << std::endl;
        SQLite::Statement q2(db, "SELECT name, age FROM users WHERE age > ?");
        q2.bind(1, 28);
        while (q2.executeStep()) {
            std::cout << "  " << q2.getColumn(0).getText()
                      << " (" << q2.getColumn(1).getInt() << ")" << std::endl;
        }

        // 6. 聚合查询
        std::cout << std::endl << "📊 Average age:" << std::endl;
        SQLite::Statement q3(db, "SELECT AVG(age) FROM users");
        if (q3.executeStep()) {
            std::cout << "  average = " << q3.getColumn(0).getDouble() << std::endl;
        }

        std::cout << std::endl << "✅ Application completed successfully!" << std::endl;

    } catch (const SQLite::Exception& e) {
        std::cerr << "❌ SQLite error: " << e.what() << std::endl;
        std::cerr << "   Error code: " << e.getErrorCode() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
