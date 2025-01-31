#ifndef TYPE_CONVERSION_UTIL
#define TYPE_CONVERSION_UTIL

#include <sstream>
#include <string>

class TypeConversionUtil {
public:
    static std::string doubleToString(double value, int precision) {
        std::ostringstream out;
        out.precision(precision);
        out << std::fixed << value;
        return out.str();
    }

private:
    TypeConversionUtil() {};
};

#endif // TYPE_CONVERSION_UTIL