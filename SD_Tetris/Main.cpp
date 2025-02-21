#include "TetrisForm.h"

using namespace System::Drawing;
using namespace System;
using namespace System::Windows::Forms;



[STAThread]
void Main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Create the main form
    TetrisForm^ form = gcnew TetrisForm();
    Application::Run(form);
}

int main(array<System::String^>^ args) {
    Main(args);
    return 0;
}
