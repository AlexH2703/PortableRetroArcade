#pragma once
#include "GameLibrary.h"
#include <iostream>

namespace PortableArcadeUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	ref class GameLibrary; // Forward declaration
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		System::Windows::Forms::Timer^ timer;
		bool isTextVisible;

	public:
		MyForm(void)
		{
			InitializeComponent();
			// Initialize and configure the Timer
			timer = gcnew System::Windows::Forms::Timer();
			timer->Interval = 750;
			timer->Tick += gcnew System::EventHandler(this, &MyForm::timer_Tick);
			isTextVisible = false;
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::Label^ textBox1;
		System::Windows::Forms::Label^ textBox2;
		System::Windows::Forms::Label^ textBox3;
		System::Windows::Forms::Label^ textBox4;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->textBox1 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::Label());
			this->textBox4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();

			int screenWidth = Screen::PrimaryScreen->Bounds.Width;
			int screenHeight = Screen::PrimaryScreen->Bounds.Height;

			this->textBox1->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 40, System::Drawing::FontStyle::Bold));
			int titleWidth1 = TextRenderer::MeasureText(L"Portable Retro", this->textBox1->Font).Width;
			this->textBox1->Location = System::Drawing::Point((screenWidth - titleWidth1) / 2, screenHeight / 6);
			this->textBox1->Text = L"Portable Retro";
			this->textBox1->AutoSize = true;
			this->textBox1->BackColor = System::Drawing::Color::Transparent;
			this->textBox1->ForeColor = System::Drawing::Color::White;

			this->textBox2->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 40, System::Drawing::FontStyle::Bold));
			int titleWidth2 = TextRenderer::MeasureText(L"Arcade", this->textBox2->Font).Width;
			this->textBox2->Location = System::Drawing::Point((screenWidth - titleWidth2) / 2, screenHeight / 4);
			this->textBox2->Text = L"Arcade";
			this->textBox2->AutoSize = true;
			this->textBox2->BackColor = System::Drawing::Color::Transparent;
			this->textBox2->ForeColor = System::Drawing::Color::White;

			this->textBox3->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 50, System::Drawing::FontStyle::Bold));
			int textWidth3 = TextRenderer::MeasureText(L"Press any Key to Start", this->textBox3->Font).Width;
			this->textBox3->Location = System::Drawing::Point((screenWidth - textWidth3) / 2, screenHeight / 2);
			this->textBox3->Text = L"Press any Key to Start";
			this->textBox3->AutoSize = true;
			this->textBox3->BackColor = System::Drawing::Color::Transparent;
			this->textBox3->ForeColor = System::Drawing::Color::White;

			this->textBox4->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 70, System::Drawing::FontStyle::Bold));
			int textWidth4 = TextRenderer::MeasureText(L"Press Esc to Exit", this->textBox4->Font).Width;
			this->textBox4->Location = System::Drawing::Point((screenWidth - textWidth4) / 2, screenHeight / 1.2);
			this->textBox4->Text = L"Press Esc to Exit";
			this->textBox4->AutoSize = true;
			this->textBox4->BackColor = System::Drawing::Color::Transparent;
			this->textBox4->ForeColor = System::Drawing::Color::White;

			this->ClientSize = System::Drawing::Size(screenWidth, screenHeight);
			this->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->KeyPreview = true;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		System::Void timer_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			textBox3->Visible = isTextVisible;
			isTextVisible = !isTextVisible;
		}

		System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
		{
			timer->Start();
		}

		System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			timer->Stop();
		}

		System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
		{
			if (e->KeyCode == Keys::Escape)
			{
				this->Close();
			}
			else
			{
				GameLibrary^ gamelibrary = gcnew GameLibrary();
				gamelibrary->Show();
			}
		}
	};
}
