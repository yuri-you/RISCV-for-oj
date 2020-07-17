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
std::deque<IF> store1;
std::deque<ID> store2;
std::deque<EX> store3;
std::deque<EX> store3_other;//�������н���
std::deque<MA> store4;
std::deque<IF> store5;//use for tomasulo  ջ
std::deque<EX>* store_ptr,* store_other;
void fun(MEM doc);
void now(command other) {
    count++;
    std::cout << std::dec << count << ' ' << std::hex << pc << ' ';
    yout(other);
    for (int i = 0; i < 32; ++i)std::cout << std::dec << x[i] << ' ';
    std::cout << std::endl;
}
int main() {
    //char name[100];
    //std::cin >> name;
    store_ptr = &(store3);
    store_other = &(store3_other);
    //name = "array_test1.data";
    MEM doc;
    //do {
    //    tmp1 = doc.fetch();
    //    tmp2 = tmp1;
    //    tmp3 = tmp2;
    //}
    //while(WB(tmp3));
    fun(doc);
    doc.dMEM();//��ֹ�������
    //predict_result();
    return 0;
}
void fun(MEM doc) {
    int number;
    IF tmp0;
    ID tmp1;
    EX tmp2;
    MA tmp3;
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
            while (!((*store_ptr).empty())) {//�ò���MA��ֱ������
                tmp2 = (*store_ptr).front();
                if (tmp2.ok())store4.push_back(tmp2);//��������ð�վͲ�����MA
                else (*store_other).push_back(tmp2);
                (*store_ptr).pop_front();
            }
            std::deque<EX>* tmp = store_ptr;
            store_ptr = store_other;
            store_other = tmp;//����store_ptr��store_other
            if (MAtime)--MAtime;
            else {
                if (!(*store_ptr).empty()) {
                    tmp2 = (*store_ptr).front();
                    (*store_ptr).pop_front();
                    store4.push_back(tmp2);
                    switch (tmp2.op) {
                    case _LH:case _LB:case _LW:case _LBU:case _LHU:--mam[tmp1.rd]; break;
                    }
                }
            }
            ++number;
        }
        if (!store2.empty()) {
            tmp1 = store2.front();
            (*store_ptr).push_back(tmp1);
            switch (tmp1.op) {
            case _LH:case _LB:case _LW:case _LBU:case _LHU:++mam[tmp1.rd]; break;
            }
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
            while (!store1.empty()&&!(store1.front().able_to_read())) {
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
        if (!finish&&(pc_of_jump==0||is_pc_forwarding)) {
            tmp0 = doc.fetch();
            store1.push_back(tmp0);
            ++number;
        }
    } while (!finish||number);
}