#include<iostream>
#include<iomanip>
#include<deque>
#include"register.hpp"
#include"instruction_fetch.hpp"
#include"instruction_decode.hpp"
#include"execute.hpp"
#include"memory_access.hpp"
#include"write_back.hpp"
using std::cin;
using std::cout;
int number;
IF tmp0,out;
ID tmp1;
EX tmp2;
MA tmp3;
std::deque<IF> store1;
std::deque<ID> store2;
std::deque<EX> store3;
std::deque<EX> store3_other;//两个队列交替
std::deque<MA> store4;
std::deque<IF> store5;//use for tomasulo  栈
std::deque<EX>* store_ptr,* store_other;
void fun(MEM doc);
void now(command other) {
    count++;
    std::cout << std::dec << count << ' ' << std::hex << pc << ' ';
    yout(other);
    for (int i = 0; i < 32; ++i)std::cout << std::dec << x[i] << ' ';
    std::cout << std::endl;
}
void dealout() {
    tmp1 = out;
    tmp2 = tmp1;
    tmp3 = tmp2;
    WB(tmp3);
    return;
}
int main() {
    store_ptr = &(store3);
    store_other = &(store3_other);
    MEM doc;
    fun(doc);
    doc.dMEM();//防止多次析构
    dealout();
    //predict_result();
    return 0;
}
void fun(MEM doc) {
    do {
        number = 0;
        if (!store4.empty()) {
            tmp3=store4.front();
            store4.pop_front();
            --wait_for_store[tmp3.rd];
            ++number;
            WB(tmp3);
            //now(tmp3.op);
        }
        if (!(*store_ptr).empty()) {
            while (!((*store_ptr).empty())) {//让不用MA的直接跳过
                tmp2 = (*store_ptr).front();
                switch (tmp2.op) {
                case _LH:case _LB:case _LW:case _LBU:case _LHU:mam[tmp1.rd] = true; break;
                }
                if (tmp2.ok())store4.push_back(tmp2);//如果会产生冒险就不跳过MA
                else (*store_other).push_back(tmp2);
                (*store_ptr).pop_front();
            }
            clearmam();
            std::deque<EX>* tmp = store_ptr;
            store_ptr = store_other;
            store_other = tmp;//交换store_ptr和store_other
            if (!(*store_ptr).empty()) {
                tmp2 = (*store_ptr).front();
                (*store_ptr).pop_front();
                store4.push_back(tmp2);
            }
            ++number;
        }
        //if (!store3.empty()) {
        //    if (MAtime)--MAtime;
        //    else {
        //        tmp2 = store3.front();
        //        store3.pop_front();
        //        if (tmp2.op != _NOT_JUMP)store4.push_back(tmp2);
        //        //else now(tmp2.op);
        //    }
        //    ++number;
        //}
        if (!store2.empty()) {
            tmp1 = store2.front();
            (*store_ptr).push_back(tmp1);
            store2.pop_front();
            ++number;
        }
        if (!store1.empty()) {
            if (popstore) {
                popstore = false;
                switch ((store1.back()).op) {
                case _BEQ:case _BLT:case _BNE:case _BGE:case _BLTU:case _BGEU:
                case _JAL:case _JALR:--pc_of_jump;
                }
                store1.pop_back();
            }
            first = true;
            while (!store1.empty()&&!(store1.front().able_to_read())) {
                first = false;
                tmp0 = store1.front();
                idm[tmp0.rst1]=true;
                idm[tmp0.rst2]=true;
                idm[tmp0.rd] = true;
                store1.pop_front();
                store5.push_back(tmp0);
            }
            if (!store1.empty()) {
                tmp0 = store1.front();
                store1.pop_front();
                store2.push_back(tmp0);
                ++wait_for_store[tmp0.rd];
            }
            for (int i = store5.size() - 1; i >= 0; --i) {
                store1.push_front(store5[i]);
            }
            store5.clear();
            clearidm();
            ++number;
        }
        //if (!store1.empty()) {
        //    if ((store1.front()).able_to_read()) {
        //        if (popstore) {
        //            popstore = false;
        //            switch ((store1.back()).op) {
        //            case _BEQ:case _BLT:case _BNE:case _BGE:case _BLTU:case _BGEU:
        //            case _JAL:case _JALR:--pc_of_jump;
        //            }
        //            store1.pop_back();
        //        }
        //        if (!store1.empty()) {
        //            tmp0 = store1.front();
        //            store1.pop_front();
        //            store2.push_back(tmp0);
        //            ++wait_for_store[tmp0.rd];
        //        }
        //    }
        //    ++number;
        //}
        if (!finish&&(pc_of_jump==0||is_pc_forwarding)) {
            tmp0 = doc.fetch();
            if (tmp0.op == _OUT) {
                out = tmp0;
            }
            else {
                store1.push_back(tmp0);
            }
            ++number;
        }
    } while (!finish||number);
}