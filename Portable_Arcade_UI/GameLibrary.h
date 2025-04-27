#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

namespace PortableArcadeUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::Windows::Forms;
	using namespace System::Resources;
	using namespace System::IO;

	/// <summary>
	/// Summary for GameLibrary
	/// </summary>
	public ref class GameLibrary : public System::Windows::Forms::Form
	{
	public:
		GameLibrary(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GameLibrary()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::PictureBox^ pictureBoxPac;
	private: System::Windows::Forms::PictureBox^ pictureSpaceInvaders;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ buttonPac;
	private: System::Windows::Forms::Button^ buttonSpace;




	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GameLibrary::typeid));
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBoxPac = (gcnew System::Windows::Forms::PictureBox());
			this->buttonPac = (gcnew System::Windows::Forms::Button());
			this->pictureSpaceInvaders = (gcnew System::Windows::Forms::PictureBox());
			this->buttonSpace = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();

			int screenWidth = Screen::PrimaryScreen->Bounds.Width;
			int screenHeight = Screen::PrimaryScreen->Bounds.Height;

			// 
			// label3
			// 
			this->label3->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 40, System::Drawing::FontStyle::Bold));
			int label3Width = TextRenderer::MeasureText(L"Portable Retro Arcade", this->label3->Font).Width;
			this->label3->Location = System::Drawing::Point((screenWidth - label3Width) / 2, screenHeight / 6);
			this->label3->Text = L"Portable Retro Arcade";
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->ForeColor = System::Drawing::Color::White;

			// 
			// label4
			// 
			this->label4->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 70, System::Drawing::FontStyle::Bold));
			int label4Width = TextRenderer::MeasureText(L"Press Esc to Exit", this->label4->Font).Width;
			this->label4->Location = System::Drawing::Point((screenWidth - label4Width) / 2, screenHeight / 1.2);
			this->label4->Text = L"Press Esc to Exit";
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->ForeColor = System::Drawing::Color::White;
			// 
			// Tetris
			// 
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox1->ImageLocation = L"";
			this->pictureBox1->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 5, Screen::PrimaryScreen->Bounds.Height / 3.6);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(Screen::PrimaryScreen->Bounds.Width / 5.5, Screen::PrimaryScreen->Bounds.Height / 4.5);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &GameLibrary::pictureBox1_Click);
			// 
			// TetrisButton
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 80, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 4.3, Screen::PrimaryScreen->Bounds.Height / 4.3);
			this->button1->Name = L"button1";
			this->button1->AutoSize = true;
			this->button1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->button1->TabIndex = 4;
			this->button1->Text = L"Tetris";
			this->button1->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &GameLibrary::button1_Click);
			this->button1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameLibrary::button1_KeyDown);
			//
			// Pacman
			// 
			this->pictureBoxPac->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pictureBoxPac->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBoxPac->ImageLocation = L"";
			this->pictureBoxPac->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 2.5, Screen::PrimaryScreen->Bounds.Height / 3.6);
			this->pictureBoxPac->Name = L"pictureBoxPac";
			this->pictureBoxPac->Size = System::Drawing::Size(Screen::PrimaryScreen->Bounds.Width / 5.5, Screen::PrimaryScreen->Bounds.Height / 4.5);
			this->pictureBoxPac->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBoxPac->TabIndex = 2;
			this->pictureBoxPac->TabStop = false;
			this->pictureBoxPac->Click += gcnew System::EventHandler(this, &GameLibrary::pac_Click);
			// 
			// buttonPac
			// 
			this->buttonPac->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 80, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPac->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 2.3, Screen::PrimaryScreen->Bounds.Height / 4.3);
			this->buttonPac->Name = L"buttonPac";
			this->buttonPac->AutoSize = true;
			this->buttonPac->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->buttonPac->TabIndex = 4;
			this->buttonPac->Text = L"Pacman";
			this->buttonPac->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			this->buttonPac->UseVisualStyleBackColor = true;
			this->buttonPac->Click += gcnew System::EventHandler(this, &GameLibrary::buttonPac_Click);
			this->buttonPac->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameLibrary::buttonPac_KeyDown);

			//
			// SpaceInvaders
			// 
			this->pictureSpaceInvaders = (gcnew System::Windows::Forms::PictureBox());
			this->pictureSpaceInvaders->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pictureSpaceInvaders->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureSpaceInvaders->ImageLocation = L"";
			this->pictureSpaceInvaders->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 1.6, Screen::PrimaryScreen->Bounds.Height / 3.6); // Adjusted location
			this->pictureSpaceInvaders->Name = L"pictureBox2";
			this->pictureSpaceInvaders->Size = System::Drawing::Size(Screen::PrimaryScreen->Bounds.Width / 5.5, Screen::PrimaryScreen->Bounds.Height / 4.5);
			this->pictureSpaceInvaders->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureSpaceInvaders->TabIndex = 2;
			this->pictureSpaceInvaders->TabStop = false;
			this->pictureSpaceInvaders->Click += gcnew System::EventHandler(this, &GameLibrary::spaceInvaders_Click);
			// 
			// buttonSpaceInvaders
			// 
			this->buttonSpace = (gcnew System::Windows::Forms::Button());
			this->buttonSpace->Font = (gcnew System::Drawing::Font(L"Press Start 2P", screenWidth / 80, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonSpace->Location = System::Drawing::Point(Screen::PrimaryScreen->Bounds.Width / 1.7, Screen::PrimaryScreen->Bounds.Height / 4.3); // Adjusted location
			this->buttonSpace->Name = L"buttonSpaceInvaders";
			this->buttonSpace->AutoSize = true;
			this->buttonSpace->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;

			this->buttonSpace->TabIndex = 4;
			this->buttonSpace->Text = L"Space Invaders";
			this->buttonSpace->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			this->buttonSpace->UseVisualStyleBackColor = true;
			this->buttonSpace->Click += gcnew System::EventHandler(this, &GameLibrary::buttonSpaceInvaders_Click);
			this->buttonSpace->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameLibrary::buttonSpaceInvaders_KeyDown);

			// 
			// GameLibrary
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(20, 19);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->CausesValidation = false;
			this->ClientSize = System::Drawing::Size(Screen::PrimaryScreen->Bounds.Width, Screen::PrimaryScreen->Bounds.Height);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->buttonPac);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->pictureBoxPac);
			this->Controls->Add(this->pictureSpaceInvaders);
			this->Controls->Add(this->buttonSpace);
			this->Cursor = System::Windows::Forms::Cursors::Hand;
			this->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"GameLibrary";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"GameLibrary";
			this->Load += gcnew System::EventHandler(this, &GameLibrary::GameLibrary_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameLibrary::GameLibrary_KeyDown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void GameLibrary_Load(System::Object^ sender, System::EventArgs^ e) {
		LoadPacmanImage();
		LoadTetrisImage(); // Call the method to load the image when the form is loaded
		LoadSpaceInvadersImage();
	}

	private: System::Void GameLibrary_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Escape) // Check if Escape key was pressed
		{
			this->Close();
		}
	}

	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchTetris();
	}
	private: System::Void pac_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchPacman();
	}
	private: System::Void spaceInvaders_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchSpaceInvaders();
	}
	private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchTetris();
	}
	private: System::Void buttonPac_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchPacman();
	}
	private: System::Void buttonSpaceInvaders_Click(System::Object^ sender, System::EventArgs^ e) {
		LaunchSpaceInvaders();
	}
	private: System::Void buttonPac_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Escape) {
			this->Close(); // Close the current form
		}
		else if (e->KeyCode == Keys::Right || e->KeyCode == Keys::Left || e->KeyCode == Keys::Up || e->KeyCode == Keys::Down) {
			LaunchPacman();
		}
	}
	private: System::Void button1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Escape) {
			this->Close(); // Close the current form
		}
		else if (e->KeyCode == Keys::Right || e->KeyCode == Keys::Left || e->KeyCode == Keys::Up || e->KeyCode == Keys::Down) {
			LaunchTetris();
		}
	}
	private: System::Void buttonSpaceInvaders_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Escape) {
			this->Close(); // Close the current form
		}
		else if (e->KeyCode == Keys::Right || e->KeyCode == Keys::Left || e->KeyCode == Keys::Up || e->KeyCode == Keys::Down) {
			LaunchSpaceInvaders();
		}
	}
		   void LoadTetrisImage() {
			   try {
				   // Get the resource manager for the current form's resources
				   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GameLibrary::typeid));
				   // Load the embedded image as a bitmap resource
				   System::Drawing::Image^ image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Tetris.png")));
				   // Set the PictureBox's Image property to the embedded image
				   if (image == nullptr) {
					   throw gcnew Exception("Image resource could not be loaded.");
				   }
				   this->pictureBox1->Image = image;
			   }
			   catch (Exception^ ex) {
				   MessageBox::Show("Error loading image: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			   }
		   }
		   void LoadPacmanImage() {
			   try {
				   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GameLibrary::typeid));
				   System::Drawing::Image^ image = Image::FromFile("Resources/pacman.png");


				   // Set the PictureBox's Image property to the embedded image
				   if (image == nullptr) {
					   throw gcnew Exception("Pacman Image resource could not be loaded.");
				   }
				   this->pictureBoxPac->Image = image;
			   }
			   catch (Exception^ ex) {
				   MessageBox::Show("Error loading image: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			   }
		   }
		   void LoadSpaceInvadersImage() {
			   try {
				   // Get the resource manager for the current form's resources
				   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GameLibrary::typeid));
				   // Load the embedded image as a bitmap resource
				   System::Drawing::Image^ image = Image::FromFile("Resources/SpaceInvaders.png");
				   // Set the PictureBox's Image property to the embedded image
				   if (image == nullptr) {
					   throw gcnew Exception("Space Invaders Image resource could not be loaded.");
				   }
				   this->pictureSpaceInvaders->Image = image;
			   }
			   catch (Exception^ ex) {
				   MessageBox::Show("Error loading image: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			   }
		   }
	private: System::Void LaunchTetris() {
		try
		{
			String^ exePath = Path::Combine(Application::StartupPath, "SD_Tetris.exe");
			System::Diagnostics::Process::Start(exePath); // Start the second project
		}
		catch (Exception^ ex)
		{
			MessageBox::Show("Failed to launch Project 2. Error: " + ex->Message,
				"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	private: System::Void LaunchPacman() {
		try
		{
			String^ exePath = Path::Combine(Application::StartupPath, "PacQt/x64/Release/PacQt.exe");
			System::Diagnostics::Process::Start(exePath); // Start the second project
		}
		catch (Exception^ ex)
		{
			MessageBox::Show("Failed to launch Project Pacman. Error: " + ex->Message,
				"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	private: System::Void LaunchSpaceInvaders() {
		try
		{
			String^ exePath = Path::Combine(Application::StartupPath, "SpaceInvaders.exe");
			System::Diagnostics::Process::Start(exePath); // Start the Space Invaders game
		}
		catch (Exception^ ex)
		{
			MessageBox::Show("Failed to launch Space Invaders. Error: " + ex->Message,
				"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	};
}
