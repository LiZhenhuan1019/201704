#include "test/test.hpp"
#include "ui/console_ui.hpp"

int main()
{
    test::test();
    ds_expr::ui::console_ui<std::string, int> ui; //vertex stores std::string, edge stores int
    ui.execute();
    return 0;
}