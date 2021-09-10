// exClassOperator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <cstring>
#include <iostream>

class Complex {
private:
    double real, img;
    double get_number(const char* str, int from, int to);
public:
    Complex(double real, double img) : real(real), img(img) {}
    Complex(const Complex& c) { real = c.real, img = c.img; }
    Complex(const char* str);
    Complex operator+(const Complex& c);
    Complex operator-(const Complex& c);
    Complex operator*(const Complex& c);
    Complex operator/(const Complex& c);
    Complex& operator+=(const Complex& c);
    Complex& operator-=(const Complex& c);
    Complex& operator*=(const Complex& c);
    Complex& operator/=(const Complex& c);
    Complex& operator=(const Complex& c);
    void println() {
        std::cout << "( " << real << " , " << img << " ) " << std::endl;
    }
};

Complex::Complex(const char* str) {
    // 입력 받은 문자열을 분석하여 real 부분과 img 부분을 찾아야 한다.
    // 문자열의 꼴은 다음과 같습니다 "[부호](실수부)(부호)i(허수부)"
    // 이 때 맨 앞의 부호는 생략 가능합니다. (생략시 + 라 가정)
    int begin = 0, end = strlen(str);
    img = 0.0;
    real = 0.0;
    // 먼저 가장 기준이 되는 'i' 의 위치를 찾는다.
    int pos_i = -1;
    for (int i = 0; i != end; i++)
    {
        if (str[i] == 'i')
        {
            pos_i = i;
            break;
        }
    }
    // 만일 'i' 가 없다면 이 수는 실수 뿐이다.
    if (pos_i == -1)
    {
            real = get_number(str, begin, end - 1);
        return;
    }
    // 만일 'i' 가 있다면, 실수부와 허수부를 나누어서 처리하면 된다.
    real = get_number(str, begin, pos_i - 1);
    img = get_number(str, pos_i + 1, end - 1);
    if (pos_i >= 1 && str[pos_i - 1] == '-') img *= -1.0;
}
double Complex::get_number(const char* str, int from, int to) {
    bool minus = false;
    if (from > to) return 0;
    if (str[from] == '-') minus = true;
    if (str[from] == '-' || str[from] == '+') from++;
    double num = 0.0;
    double decimal = 1.0;
    bool integer_part = true;
    for (int i = from; i <= to; i++)
    {
        if (isdigit(str[i]) && integer_part)
        {
            num *= 10.0;
            num += (str[i] - '0');
        }
        else if (str[i] == '.')
            integer_part = false;
        else if (isdigit(str[i]) && !integer_part)
        {
            decimal /= 10.0;
            num += ((str[i] - '0') * decimal);
        }
        else
            break; // 그 이외의 이상한 문자들이 올 경우
    }
    if (minus) num *= -1.0;
    return num;
}
Complex Complex::operator+(const Complex& c) {
    Complex temp(real + c.real, img + c.img);
    return temp;
}
Complex Complex::operator-(const Complex& c) {
    Complex temp(real - c.real, img - c.img);
    return temp;
}
Complex Complex::operator*(const Complex& c) {
    Complex temp(real * c.real - img * c.img, real * c.img + img * c.real);
    return temp;
}
Complex Complex::operator/(const Complex& c) {
        Complex temp(
            (real * c.real + img * c.img) / (c.real * c.real + c.img * c.img),
            (img * c.real - real * c.img) / (c.real * c.real + c.img * c.img));
    return temp;
}
Complex& Complex::operator+=(const Complex& c) {
    (*this) = (*this) + c;
    return *this;
}
Complex& Complex::operator-=(const Complex& c) {
    (*this) = (*this) - c;
    return *this;
}
Complex& Complex::operator*=(const Complex& c) {
    (*this) = (*this) * c;
    return *this;
}
Complex& Complex::operator/=(const Complex& c) {
    (*this) = (*this) / c;
    return *this;
}
Complex& Complex::operator=(const Complex& c) {
    real = c.real;
    img = c.img;
    return *this;
}
int main() {
    Complex a(0, 0);
    a = a + "-1.1 + i3.923";
    a.println();
    a = a - "1.2 -i1.823";
    a.println();
    a = a * "2.3+i22";
    a.println();
    a = a / "-12+i55";
    a.println();
}


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
