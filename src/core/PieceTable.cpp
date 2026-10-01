#include <cstddef>
#include <onix/core/PieceTable.hpp>
#include <string>
#include <string_view>

namespace onix::core {
    
PieceTable::PieceTable(std::string_view initial_text){
    if(!initial_text.empty()){
        original_buffer_ = std::string(initial_text);
        pieces_.push_back(Piece{
            .buffer = BufferType::Original,
            .start = 0,
            .length = initial_text.size()
        });
        total_size_ = initial_text.size();
    }
}
    
void PieceTable::insert(std::size_t offset, std::string_view text){
    (void)offset;
    (void)text;
}

void PieceTable::remove(std::size_t offset, std::size_t length){
    (void)offset;
    (void)length;
}

std::size_t PieceTable::size() const noexcept{
    return total_size_;
}
    
bool PieceTable::empty() const noexcept{
    return total_size_ == 0;
}

std::string PieceTable::get_text() const{
    std::string result;
    result.reserve(total_size_);
    
    for(const auto& piece : pieces_){
        const std::string& source = (piece.buffer == BufferType::Original) ? original_buffer_ : add_buffer_;
        result.append(source, piece.start, piece.length);
    }
    
    return result;
}

}