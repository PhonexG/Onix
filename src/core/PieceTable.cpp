#include <cstddef>
#include <onix/core/PieceTable.hpp>
#include <stdexcept>
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
    if (text.empty()) return;
    if (offset > total_size_) throw std::out_of_range("Offset out of range");

    std::size_t start_in_add = add_buffer_.size();
    add_buffer_.append(text);
    Piece new_piece{BufferType::Add, start_in_add, text.size()};

    if (pieces_.empty() || offset == total_size_) {
        pieces_.push_back(new_piece);
        total_size_ += text.size();
        return;
    }

    std::size_t current_offset = 0;
    for (auto it = pieces_.begin(); it != pieces_.end(); ++it) {
        if (offset <= current_offset + it->length) {
            std::size_t split_point = offset - current_offset;
            Piece original = *it;

            it = pieces_.erase(it);

            if (split_point < original.length) {
                it = pieces_.insert(it, Piece{
                    .buffer = original.buffer,
                    .start = original.start + split_point,
                    .length = original.length - split_point
                });
            }
            it = pieces_.insert(it, new_piece);

            if (split_point > 0) {
                pieces_.insert(it, Piece{
                    .buffer = original.buffer,
                    .start = original.start,
                    .length = split_point
                });
            }
            total_size_ += text.size();
            return;
        }
        current_offset += it->length;
    }
}

void PieceTable::remove(std::size_t offset, std::size_t length){
    if (length == 0) return;
    if ((offset >= total_size_) || (offset + length > total_size_))
        throw std::out_of_range("Offset out of range");

    std::size_t current_pos = 0;
    std::size_t rem_end = offset + length;
    std::vector<Piece> new_pieces;

    for (const auto& piece : pieces_) {
        std::size_t p_start = current_pos;
        std::size_t p_end = current_pos + piece.length;

        if (p_start < offset) new_pieces.push_back(Piece{
            .buffer = piece.buffer,
            .start = piece.start,
            .length = std::min(piece.length, offset - p_start)
        });

        std::size_t cut = (rem_end > p_start) ? (rem_end - p_start) : 0;
        if (p_end > rem_end) new_pieces.push_back(Piece{
            .buffer = piece.buffer,
            .start = piece.start + cut,
            .length = piece.length - cut
        });

        current_pos += piece.length;
    }
    pieces_ = std::move(new_pieces);
    total_size_ -= length;

    return;
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
