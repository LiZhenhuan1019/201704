#include "test/test.hpp"
#include "ui/console_ui.hpp"

int main()
{
    test::test();
    ds_expr::console_ui<std::string, std::string> ui;
    ui.execute();
    return 0;
}