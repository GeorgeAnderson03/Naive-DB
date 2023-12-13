#include "db.hpp"

void Database::CreateTable(const std::string& table_name) {
  DbTable* temp = new DbTable();
  tables_.insert(std::pair<std::string, DbTable*>(table_name, temp));
}
void Database::DropTable(const std::string& table_name) {
  if (!(tables_.contains(table_name))) {
    throw std::invalid_argument("a");
  }
  tables_.at(table_name)->~DbTable();
}
DbTable& Database::GetTable(const std::string& table_name) {
  DbTable* temp = nullptr;
  temp = tables_.at(table_name);
  return *temp;
}

Database::Database(const Database& rhs) { *this = rhs; }
Database& Database::operator=(const Database& rhs) {
  if (this == &rhs) {
    return *this;
  }
  return *this;
}
