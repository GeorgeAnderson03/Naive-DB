#include "db_table.hpp"

const unsigned int kRowGrowthRate = 2;

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
  if (col_descs_.size() == row_col_capacity_) {
    for (unsigned int i = 0; i < next_unique_id_; i++) {
      if (rows_.contains(i)) {
        void** temp = new void*[row_col_capacity_ * kRowGrowthRate];
        for (unsigned int j = 0; j < col_descs_.size(); j++) {
          temp[j] = rows_.at(i)[j];
        }
        for (unsigned int k = col_descs_.size();
             k < row_col_capacity_ * kRowGrowthRate;
             k++) {
          temp[k] = nullptr;
        }
        delete[] rows_.at(i);
        rows_.at(i) = temp;
      }
    }

    row_col_capacity_ = row_col_capacity_ * kRowGrowthRate;
    // delete temp? couldnt do for loop
    // local testing issues?
    // resizing issues and same with delete column if logic is good?
  }
  col_descs_.push_back(col_desc);
  for (unsigned int i = 0; i < next_unique_id_; i++) {
    if (col_desc.second == DataType::kString) {
      rows_.at(i)[col_descs_.size() - 1] =
          static_cast<void*>(new std::string(""));
    }
    if (col_desc.second == DataType::kInt) {
      rows_.at(i)[col_descs_.size() - 1] = static_cast<void*>(new int(0));
    }
    if (col_desc.second == DataType::kDouble) {
      rows_.at(i)[col_descs_.size() - 1] = static_cast<void*>(new double(0.0));
    }
  }
}
void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
  if (col_idx < 0 || col_idx >= col_descs_.size()) {
    throw std::out_of_range("a");
  }
  if (col_descs_.size() == 1 && !(rows_.empty())) {
    throw std::runtime_error(
        "any table with at least one row must have at least one column.");
  }
  for (unsigned int i = 0; i < next_unique_id_; i++) {
    if (rows_.contains(i)) {
      if (col_descs_.at(col_idx).second == DataType::kString) {
        delete (static_cast<std::string*>(rows_.at(i)[col_idx]));
      }
      if (col_descs_.at(col_idx).second == DataType::kInt) {
        delete (static_cast<int*>(rows_.at(i)[col_idx]));
      }
      if (col_descs_.at(col_idx).second == DataType::kDouble) {
        delete (static_cast<double*>(rows_.at(i)[col_idx]));
      }
      for (unsigned int j = col_idx; j < col_descs_.size() - 1; j++) {
        rows_.at(i)[j] = rows_.at(i)[j + 1];
      }
      rows_.at(i)[col_descs_.size() - 1] = nullptr;
    }
  }
  col_descs_.erase(col_descs_.begin() + col_idx);
  std::cout << col_descs_.size() << std::endl;
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
  if (col_data.size() != col_descs_.size()) {
    throw std::invalid_argument("A");
  }
  void** temp = new void*[row_col_capacity_];
  int counter = 0;  // was doing for loop dont think implementation is correct
  for (const std::string& ah : col_data) {
    if (col_descs_.at(counter).second == DataType::kDouble) {
      temp[counter] = static_cast<void*>(new double(std::stod(ah)));
    } else if (col_descs_.at(counter).second == DataType::kInt) {
      temp[counter] = static_cast<void*>(new int(std::stoi(ah)));
    } else if (col_descs_.at(counter).second == DataType::kString) {  // string
      temp[counter] = static_cast<void*>(new std::string(ah));
    }
    counter++;
  }
  for (unsigned int i = col_descs_.size(); i < row_col_capacity_;
       i++) {  // add the null ptr bits between coldescs and row col capacity
    temp[i] = nullptr;
  }
  rows_.insert(std::pair<int, void**>(next_unique_id_, temp));
  next_unique_id_++;
  // delete temp?
}
void DbTable::DeleteRowById(unsigned int id) {
  if (!(rows_.contains(id))) {
    throw std::invalid_argument("A");
  }

  for (unsigned int i = 0; i < col_descs_.size(); i++) {
    if (col_descs_.at(i).second == DataType::kString) {
      delete (static_cast<std::string*>(rows_.at(id)[i]));
    }
    if (col_descs_.at(i).second == DataType::kInt) {
      delete (static_cast<int*>(rows_.at(id)[i]));
    }
    if (col_descs_.at(i).second == DataType::kDouble) {
      delete (static_cast<double*>(rows_.at(id)[i]));
    }
  }
  delete[] rows_.at(id);  // delete whole row after elements
  rows_.erase(
      id);  // erase id from maps but i think there are issues with erase
}

DbTable::DbTable(const DbTable& rhs) {
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  for (unsigned int i = 0; i < next_unique_id_; i++) {
    if (rhs.rows_.contains(i)) {
      rows_.insert(std::pair<unsigned int, void**>(i, rhs.rows_.at(i)));
    }
  }
  for (unsigned int i = 0; i < col_descs_.size(); i++) {
    col_descs_.push_back(rhs.col_descs_.at(i));
  }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
  if (this == &rhs) {
    return *this;
  }
  // void* temp = nullptr;
  // temp = new void*[rhs.row_col_capacity_];
  //  for (int i = 0; i < rhs.next_unique_id_; i++) {

  //}

  return *this;
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
  for (unsigned int i = 0; i < table.col_descs_.size(); i++) {
    if (!(i == table.col_descs_.size() - 1)) {
      if (table.col_descs_.at(i).second == DataType::kDouble) {
        os << table.col_descs_.at(i).first << "(std::string), ";
      }
      if (table.col_descs_.at(i).second == DataType::kInt) {
        os << table.col_descs_.at(i).first << "(int), ";
      }
      if (table.col_descs_.at(i).second == DataType::kDouble) {
        os << table.col_descs_.at(i).first << "(double), ";
      }
    } else {
      if (table.col_descs_.at(i).second == DataType::kDouble) {
        os << table.col_descs_.at(i).first << "(std::string)";
      }
      if (table.col_descs_.at(i).second == DataType::kInt) {
        os << table.col_descs_.at(i).first << "(int)";
      }
      if (table.col_descs_.at(i).second == DataType::kDouble) {
        os << table.col_descs_.at(i).first << "(double)";
      }
    }
  }  // should be just the first line of the output done
  os << std::endl;
  for (unsigned int rows = 0; rows < table.next_unique_id_; rows++) {
    for (unsigned int col = 0; col < table.col_descs_.size(); col++) {
      if (!(rows == table.next_unique_id_ - 1)) {
        if (table.col_descs_.at(col).second == DataType::kDouble) {
          os << (static_cast<double*>(table.rows_.at(rows)[col])) << ", ";
        }
        if (table.col_descs_.at(col).second == DataType::kInt) {
          os << (static_cast<int*>(table.rows_.at(rows)[col])) << ", ";
        }
        if (table.col_descs_.at(col).second == DataType::kString) {
          os << (static_cast<std::string*>(table.rows_.at(rows)[col])) << ", ";
        }
        os << std::endl;
      } else {
        if (!(rows == table.next_unique_id_ - 1)) {
          if (table.col_descs_.at(col).second == DataType::kDouble) {
            os << (static_cast<double*>(table.rows_.at(rows)[col]));
          }
          if (table.col_descs_.at(col).second == DataType::kInt) {
            os << (static_cast<int*>(table.rows_.at(rows)[col]));
          }
          if (table.col_descs_.at(col).second == DataType::kString) {
            os << (static_cast<std::string*>(table.rows_.at(rows)[col]));
          }
        }
      }
    }
  }
  return os;
}
DbTable::~DbTable() {
  for (unsigned int row_id = 0; row_id < next_unique_id_; row_id++) {
    if (rows_.contains(row_id)) {
      DeleteRowById(row_id);  // is there anymore to this?
    }
  }
}