#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

int main() {
  DbTable temp;
  temp.AddColumn({"Top", DataType::kInt});
  temp.AddColumn({"Top", DataType::kInt});
  temp.AddColumn({"Top", DataType::kInt});
  temp.DeleteColumnByIdx(2);
  temp.DeleteColumnByIdx(1);
  temp.DeleteColumnByIdx(0);
  temp.DeleteRowById(1);
}