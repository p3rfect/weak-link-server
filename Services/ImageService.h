//
// Created by p3rfect on 20.06.24.
//

#ifndef WEAKLINK_IMAGESERVICE_H
#define WEAKLINK_IMAGESERVICE_H

#include <fstream>

class ImageService {
public:
    static bool save_image(const std::string& filename, const std::string& data);
    static std::string encode_image(const std::string& filename);
private:
    inline static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    static inline bool is_base64(unsigned char c);
    static std::string base64_decode(const std::string &encoded_string);
    static std::string base64_encode(const std::string &data);
};


#endif //WEAKLINK_IMAGESERVICE_H
