//
// Created by ytymchenko on 07.11.2021.
//

#ifndef LOGGIO_CONCURRENT_MAP_H
#define LOGGIO_CONCURRENT_MAP_H

#include <map>
#include <mutex>
#include <string>
#include <sstream>


template<typename V>
class concurrent_map {
private:
    std::map<size_t, V> cm;
    std::mutex m;
public:

    concurrent_map() = default;

    void push(V value) {
        {
            std::lock_guard<std::mutex> lck(m);
            cm[cm.size() + 1] = value;
        }
    }

    std::string get_map(std::string &&comment) {
        {
            std::lock_guard<std::mutex> lck(m);
            std::stringstream ss;
            ss << comment;
            for (const auto& item : cm) {
                ss << item.first << " : " << item.second << "; ";
            }
            ss << "\n";
            return std::move(ss.str());
        }
    }

    size_t size() {
        {
            std::lock_guard<std::mutex> lck(m);
            return cm.size();
        }
    }

};


#endif //LOGGIO_CONCURRENT_MAP_H
