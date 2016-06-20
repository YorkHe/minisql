#include "utility.h"
#include <vector>
#include <ostream>
class RecordManager{
public:
    CError selectRecord(Condition cond);

    CError deleteRecord(Condition cond);

    CError insertRecord(Tuple tuple);

    void printAll(std::ostream &out);

    void printLimit(std::ostream &out, int from, int count);
private:
    std::vector<Element> result;
};