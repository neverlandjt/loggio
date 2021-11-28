//
// Created by ytymchenko on 07.11.2021.
//

#ifndef LOGGIO_CONCURRENT_MAP_H
#define LOGGIO_CONCURRENT_MAP_H

#include <map>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>


template<typename V>
class concurrent_map {
private:
    std::map<size_t, V> cm;
    std::mutex m;

    struct order_data {
        std::vector<size_t> missed_idx{1};
        std::vector<size_t> idx{};
        bool current = false;

        order_data() = default;

        void operator()(size_t const &item) {
            auto current_id = std::find(missed_idx.begin(), missed_idx.end(), item);
            current = current_id != missed_idx.end();

            if (std::find(idx.begin(), idx.end(), item + 1) == idx.end()) {
                if (current)
                    *current_id = item + 1;
                else
                    missed_idx.push_back(item + 1);
            } else if (current) {
                missed_idx.erase(current_id);
            }
            idx.push_back(item);
        }

        size_t get_last_sequential_idx() {
            return missed_idx[0] - 1;
        }

    };

    order_data order;
public:


    concurrent_map() = default;

    int push(V value) {
        {
            std::lock_guard<std::mutex> lck(m);
            const size_t current_id = cm.size() + 1;
            cm[current_id] = value;
            order(current_id);
            return cm.size() - 1;
        }
    }


    void push_pos(V value, size_t current_id) {
        {
            std::lock_guard<std::mutex> lck(m);
            if (cm.find(current_id) == cm.end()) {
                cm[current_id] = value;
                order(current_id);
            } else {
                std::cout << "Skipping id [" << current_id << "] - duplicate" << std::endl;
            }

        }
    }

    std::string get_map(std::string &&comment) {
        {
            std::lock_guard<std::mutex> lck(m);
            std::stringstream ss;
            ss << comment<<" ";
            for (const auto &item : cm) {
                ss <<"( "<< item.first << " : " << item.second << " ) ";
                if (item.first == order.get_last_sequential_idx()) break;
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
