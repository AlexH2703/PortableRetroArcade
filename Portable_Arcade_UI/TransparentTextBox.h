#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class TransparentTextBox : public TextBox
{
public:
    TransparentTextBox()
    {
        this->SetStyle(ControlStyles::SupportsTransparentBackColor, true);
        this->BackColor = Color::Transparent;
        this->BorderStyle = System::Windows::Forms::BorderStyle::None; // Optional
        this->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 12, FontStyle::Regular));
    }

protected:
    virtual void OnPaintBackground(PaintEventArgs^ e) override
    {
        // Do not paint the background
    }

    virtual void OnPaint(PaintEventArgs^ e) override
    {
        e->Graphics->Clear(Color::Transparent);
        TextBox::OnPaint(e);
    }
};