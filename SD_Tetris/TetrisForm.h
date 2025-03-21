#pragma once
#include <cmath>
#include "GameBoard.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Data;
using namespace System::Data::SqlClient;

public ref class TetrisForm : public Form {
private:
    /*-------------------------------------------------------------------------
                             Variable Declaration
    -------------------------------------------------------------------------*/
    Panel^ outerPanel; // Outer panel for borders
    Panel^ boardPanel; // Panel to represent the Tetris board
    const int blockSize = 25; // Size of each block
    const int rows = 20; // Number of rows in the Tetris board
    const int cols = 10; // Number of columns in the Tetris board
    const int borderThickness = 15; // Thickness of the border
    Bitmap^ backgroundImage; // Background image
    String^ connectionString = "Server=localhost; Database=PORTABLE_RETRO_ARCADE; Integrated Security=True;";
    System::Windows::Forms::Label^ label4;  //Press Esc to Exit
    int currentRotation = 0;
    array<array<Point>^>^ tShapeCoords; //holds the coords for the falling shape
    int shapeType;
    Random^ rand;
    
    Point shapePosition; // Current position of the T shape
    Color shapeColor; // Color of the shape
    
    Rectangle shape; // Rectangle to represent the movable shape
    GameBoard^ gameBoard;
    System::Windows::Forms::Timer^ fallTimer; // Timer for falling blocks
    System::Windows::Forms::Label^ gameOverLabel; // Label for game over message
    bool isGameOver; // Flag to track game over status

    //Variables for hold shape and next shape
    // int heldShapeType = 0;            // Shape type for the held shape
    bool canHold = true;               // Flag to prevent multiple holds in one turn
    bool hasHeldPiece; // Flag to check if the piece has been held
    int holdShapeType; // The type of the held shape
    Point holdPosition; // The position of the hold piece
    Color holdShapeColor; // The color of the held shape
    int nextShapeType; // Type of the next shape
    Color nextShapeColor; // Color of the next shape
    int fallInterval = 1000;
    int level = 0;

    //Panel and Label Declarations
    Panel^ overlayPanel; // Overlay panel for game over
    Button^ restartButton;
    Button^ mainMenuButton;

    Panel^ scorePanel; // Panel for score display
    Label^ linesValueLabel; // Label for displaying score
    Label^ scoreValueLabel; // Label for displaying level
    Label^ levelValueLabel; // Label for displaying lines cleared
    Label^ holdLabel; // Label for displaying lines cleared
    Label^ nextLabel; // Label for displaying lines cleared

    Panel^ holdPanel; // Panel to display the held shape
    Panel^ nextPanel; // Panel to display the next shape

    Panel^ highScorePanel; // Panel to display high scores
    Label^ highScoreHeaderLabel; // Label for displaying lines cleared
    Panel^ NewHighScorePanel;
    Label^ NewHighScoreHeaderLabel;
    Label^ EnterInitalsLabel;
    TextBox^ EnterInitals;
    Label^ DisplayHighScoreHeader;
    Label^ DisplayHighScore;
    Button^ NewHighScoreButton;
    Button^ NewHighScoreCancelButton;

    //Image Paths
    String^ squareImagePath;
    String^ lineImagePath;
    String^ timagePath;
    String^ simagePath;
    String^ zimagePath;
    String^ limagePath;
    String^ jimagePath;
    String^ backgroundPath;
    //Shape Pictures
    PictureBox^ squarePicture;
    PictureBox^ linePicture;
    PictureBox^ tPicture;
    PictureBox^ sPicture;
    PictureBox^ zPicture;
    PictureBox^ lPicture;
    PictureBox^ jPicture;

    /*-------------------------------------------------------------------------
                         Initalize Shape Coords
    -------------------------------------------------------------------------*/
    array<array<Point>^>^ InitializeTShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point> { Point(0, 0), Point(-1, 0), Point(1, 0), Point(0, -1) }, // 0 degrees
                gcnew array<Point> { Point(0, 0), Point(0, -1), Point(0, 1), Point(1, 0) }, // 90 degrees
                gcnew array<Point> { Point(0, 0), Point(-1, 0), Point(1, 0), Point(0, 1) }, // 180 degrees
                gcnew array<Point> { Point(0, 0), Point(0, -1), Point(0, 1), Point(-1, 0) }  // 270 degrees
        };
    }

    array<array<Point>^>^ InitializeSquareShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1) },
                gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1) },
                gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1) },
                gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1) }

        };
    }

    array<array<Point>^>^ InitializeLineShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 0), Point(0, 1), Point(0, 2), Point(0, 3) }, // 0 degrees
                gcnew array<Point>{ Point(0, 0), Point(-1, 0), Point(-2, 0), Point(-3, 0) },  // 270 degrees
                gcnew array<Point>{ Point(0, 0), Point(0, -1), Point(0, -2), Point(0, -3) }, // 0 degrees
                gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0) },  // 270 degrees
        };
    }

    array<array<Point>^>^ InitializeSShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 1), Point(1, 0), Point(1, 1), Point(2, 0) }, // 0 degrees
                gcnew array<Point>{ Point(1, 0), Point(1, 1), Point(2, 1), Point(2, 2) },  // 270 degrees
                gcnew array<Point>{ Point(2, 1), Point(1, 1), Point(1, 2), Point(0, 2) }, // 0 degrees
                gcnew array<Point>{ Point(0, 0), Point(0, 1), Point(1, 1), Point(1, 2) },  // 270 degrees

        };
    }

    array<array<Point>^>^ InitializeZShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 0), Point(1, 0), Point(1, 1), Point(2, 1) }, // 0 degrees
                gcnew array<Point>{ Point(1, -1), Point(1, 0), Point(0, 0), Point(0, 1) },  // 270 degrees
                gcnew array<Point>{ Point(2, 0), Point(1, 0), Point(1, -1), Point(0, -1) }, // 0 degrees
                gcnew array<Point>{ Point(1, 1), Point(1, 0), Point(2, 0), Point(2, -1) },  // 270 degrees

        };
    }

    array<array<Point>^>^ InitializeLShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 0), Point(0, 1), Point(0, 2), Point(1, 2) }, // 0 degrees
                gcnew array<Point>{ Point(0, 3), Point(2, 2), Point(0, 2), Point(1, 2) },  // 270 degrees
                gcnew array<Point>{ Point(0, 4), Point(0, 3), Point(0, 2), Point(-1, 2) }, // 0 degrees
                gcnew array<Point>{ Point(-2, 2), Point(-1, 2), Point(0, 2), Point(0, 1) },  // 270 degrees

        };
    }

    array<array<Point>^>^ InitializeJShape() {
        shapePosition = Point(cols / 2, 0); // Start in the middle at the top
        return gcnew array<array<Point>^> {
            gcnew array<Point>{ Point(0, 1), Point(0, 0), Point(-1, 2), Point(0, 2) }, // 0 degrees
                gcnew array<Point>{ Point(0, 1), Point(2, 2), Point(0, 2), Point(1, 2) },  // 270 degrees
                gcnew array<Point>{ Point(0, 4), Point(0, 3), Point(0, 2), Point(1, 2) }, // 0 degrees
                gcnew array<Point>{ Point(-2, 2), Point(-1, 2), Point(0, 2), Point(0, 3) },  // 270 degrees

        };
    }

    /*-------------------------------------------------------------------------
                              Modify Falling Shape
    -------------------------------------------------------------------------*/

    bool CanRotate() {
        int originalRotation = currentRotation;
        currentRotation = (currentRotation + 1) % 4; // Attempt to rotate
        for each (Point p in tShapeCoords[currentRotation]) {
            int x = shapePosition.X + p.X;
            int y = shapePosition.Y + p.Y;
            // Check boundaries
            if (x < 0 || x >= cols || y < 0 || y >= rows) {
                currentRotation = originalRotation; // Revert if out of bounds
                return false;
            }
        }
        currentRotation = originalRotation; // Revert to the original rotation
        return true; // Can rotate
    }

    void RotateShape() {
        if (CanRotate()) {
            currentRotation = (currentRotation + 1) % 4; // Perform the rotation
        }
    }

    Point GetLandingPosition() {
        Point landingPosition = shapePosition;

        while (gameBoard->CanPlacePiece(landingPosition.X, landingPosition.Y + 1, tShapeCoords[currentRotation])) {
            landingPosition.Y += 1; // Move down until it can't
        }

        return landingPosition; // Return the final landing position
    }

    Point CalculateGhostPosition() {
        Point ghostPosition = shapePosition;

        while (gameBoard->CanPlacePiece(ghostPosition.X, ghostPosition.Y + 1, tShapeCoords[currentRotation])) {
            ghostPosition.Y += 1; // Move down until it can't
        }

        return ghostPosition; // Return the position where the shape will land
    }

    /*-------------------------------------------------------------------------
                                 Spawn New Shape
    -------------------------------------------------------------------------*/
    void SpawnNewShape() {
        shapeType = nextShapeType;
        shapeColor = nextShapeColor;
        shapePosition = Point(cols / 2, 0); // Start at the top center
        currentRotation = 0;

        // Get the next shape
        nextShapeType = rand->Next(1, 8); // Random next shape
        nextShapeColor = GetShapeColor(nextShapeType); // Get the next shape's color
        DrawNextShape();
        DrawHoldShape();

        // Initialize the shape based on the generated shapeType
        switch (shapeType) {
        case 1: tShapeCoords = InitializeTShape(); break;
        case 2: tShapeCoords = InitializeSquareShape(); break;
        case 3: tShapeCoords = InitializeLineShape(); break;
        case 4: tShapeCoords = InitializeSShape(); break;
        case 5: tShapeCoords = InitializeZShape(); break;
        case 6: tShapeCoords = InitializeLShape(); break;
        case 7: tShapeCoords = InitializeJShape(); break;
        }
        // Check if the shape can fit, if not adjust position
        while (true) {
            bool isShapeValid = true;
            for each (Point p in tShapeCoords[currentRotation]) {
                int x = shapePosition.X + p.X;
                int y = shapePosition.Y + p.Y;
                if (y < 0 || y >= rows) { // Check boundaries
                    isShapeValid = false;
                    break;
                }
            }
            if (isShapeValid) {
                break; // Shape is valid, exit the loop
            }
            else {
                shapePosition.Y += 1; // Move down if invalid
            }
        }
    }

    Color GetShapeColor(int blockType) {
        // Define colors based on the block type
        switch (blockType) {
        case 1: return Color::LimeGreen;
        case 2: return Color::LightCoral;
        case 3: return Color::LightSkyBlue;
        case 4: return Color::DeepPink;
        case 5: return Color::DarkGreen;
        case 6: return Color::MediumBlue;
        case 7: return Color::Orchid;
        default: return Color::Transparent;
        }
    }

    /*-------------------------------------------------------------------------
                                 Button Functions
    -------------------------------------------------------------------------*/

    void DrawButtonFocus(Graphics^ g, Rectangle buttonRect) {
        // Create a custom pen for the focus rectangle
        Pen^ focusPen = gcnew Pen(Color::LightSkyBlue, 6); // Change color and thickness as needed

        // Draw a rectangle around the button
        g->DrawRectangle(focusPen, buttonRect);
        delete focusPen; // Clean up
    }

    // Create the Paint event handler
    void OnButtonPaint(Object^ sender, PaintEventArgs^ e) {
        Button^ button = dynamic_cast<Button^>(sender);
        if (button->Focused) { // Check if the button is focused
            DrawButtonFocus(e->Graphics, button->ClientRectangle);
        }
    }

    /*-------------------------------------------------------------------------
                                Score Functions
    -------------------------------------------------------------------------*/
    System::Void UpdateScore(int newScore) {
        if (scoreValueLabel != nullptr) {
            if (scoreValueLabel->InvokeRequired) {
                scoreValueLabel->Invoke(gcnew System::Action<int>(this, &TetrisForm::UpdateScore), newScore);
            }
            else {
                scoreValueLabel->Text = newScore.ToString();
            }
        }
        Console::Write("Updating Score: " + newScore);
    }

    System::Void UpdateLevel(int newLevel) {
        if (levelValueLabel != nullptr) {
            if (levelValueLabel->InvokeRequired) {
                levelValueLabel->Invoke(gcnew System::Action<int>(this, &TetrisForm::UpdateLevel), newLevel);
            }
            else {
                levelValueLabel->Text = newLevel.ToString();
            }
            if (newLevel != level) {
                int speedChange = fallInterval / 3;
                fallInterval -= speedChange;
                fallTimer->Interval = fallInterval;
                level = newLevel;
            }
        }
        Console::Write("Updating Level: " + newLevel);
    }

    System::Void UpdateLines(int linesCleared) {
        if (linesValueLabel != nullptr) {
            if (linesValueLabel->InvokeRequired) {
                linesValueLabel->Invoke(gcnew System::Action<int>(this, &TetrisForm::UpdateLines), linesCleared);
            }
            else {
                linesValueLabel->Text = linesCleared.ToString();
            }
        }
        Console::Write("Updating Lines: " + linesCleared);
    }


public:
    TetrisForm() {
        this->Text = "Tetris Game";

        backgroundPath = System::IO::Path::Combine(Application::StartupPath, "Resources\\Arcade_background.jpg");
        if (System::IO::File::Exists(backgroundPath))
        {
            backgroundImage = gcnew System::Drawing::Bitmap(backgroundPath);
        }
        else
        {
            MessageBox::Show("Background image not found at: " + backgroundPath, "File Not Found", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        /*--------------------------------------------------------------------------------------------------------------------------------------------
                                                               
                                                               Initalize Objects

        --------------------------------------------------------------------------------------------------------------------------------------------*/

        /*-------------------------------------------------------------------------
                             Form
        -------------------------------------------------------------------------*/
        // Set form size
        this->Size = System::Drawing::Size(Screen::PrimaryScreen->Bounds.Width, Screen::PrimaryScreen->Bounds.Height);
        this->StartPosition = FormStartPosition::CenterScreen; // Center the form on the screen
        this->DoubleBuffered = true; // Reduces flickering
        this->WindowState = FormWindowState::Maximized; // Optional: Start in full screen
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None; // Remove the border
        // Using GetObject to get the image from resources

        gameBoard = gcnew GameBoard(); // Initialize the GameBoard

        // Label setup
        this->label4 = (gcnew System::Windows::Forms::Label());
        this->label4->AutoSize = true;
        this->label4->BackColor = System::Drawing::Color::Transparent;
        this->label4->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 14, System::Drawing::FontStyle::Bold));
        this->label4->ForeColor = System::Drawing::SystemColors::ActiveBorder;
        this->label4->Location = System::Drawing::Point(500, 660);
        this->label4->Name = L"label4";
        this->label4->TabIndex = 1;
        this->label4->Text = L"Press Esc to Exit";
        this->Controls->Add(label4);

        /*-------------------------------------------------------------------------
                             Game Board
        -------------------------------------------------------------------------*/
        // Create and configure the outer panel for borders
        outerPanel = gcnew Panel();
        outerPanel->Size = System::Drawing::Size(cols * blockSize + 2 * borderThickness, rows * blockSize + 2 * borderThickness);
        outerPanel->Location = System::Drawing::Point((this->ClientSize.Width - outerPanel->Width) / 1.85, (this->ClientSize.Height - outerPanel->Height) / 2);
        outerPanel->BackColor = Color::Transparent; // Set background color to transparent
        this->Controls->Add(outerPanel);

        // Create and configure the  Tetris board panel
        boardPanel = gcnew Panel();
        boardPanel->Size = System::Drawing::Size(cols * blockSize, rows * blockSize);
        boardPanel->Location = System::Drawing::Point(borderThickness, borderThickness);
        boardPanel->Paint += gcnew PaintEventHandler(this, &TetrisForm::DrawGrid); // Attach paint event handler
        boardPanel->BackColor = Color::White; // Set the background color
        outerPanel->Controls->Add(boardPanel);
        
        fallTimer = gcnew System::Windows::Forms::Timer();
        fallTimer->Interval = fallInterval; // Set interval to 1000 ms (1 second) for falling speed
        fallTimer->Tick += gcnew EventHandler(this, &TetrisForm::OnFallTimerTick);
        fallTimer->Start(); // Start the timer


        /*-------------------------------------------------------------------------
                                        Game Over
        -------------------------------------------------------------------------*/
        // Initialize overlayPanel
        overlayPanel = gcnew Panel();
        overlayPanel->Size = System::Drawing::Size(400, 175); // Full size
        overlayPanel->Location = System::Drawing::Point(
            (this->ClientSize.Width - overlayPanel->Width) / 2,
            (this->ClientSize.Height - overlayPanel->Height) / 2
        );
        overlayPanel->BackColor = Color::DarkGray;
        overlayPanel->Visible = false; // Initially hidden
        this->Controls->Add(overlayPanel);

        // Initialize game over label
        gameOverLabel = gcnew System::Windows::Forms::Label();
        gameOverLabel->AutoSize = true;
        gameOverLabel->BackColor = Color::Transparent;
        gameOverLabel->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 28, System::Drawing::FontStyle::Bold));
        gameOverLabel->ForeColor = Color::Black; // Game over text color
        gameOverLabel->Text = "Game Over!";
        gameOverLabel->Location = Point((overlayPanel->Width - gameOverLabel->Width) / 35,
            (overlayPanel->Height - 150)
        );
        gameOverLabel->Visible = true; // Set visibility true when added
        overlayPanel->Controls->Add(gameOverLabel);

        // Initialize Restart Button
        restartButton = gcnew Button();
        restartButton->Text = "Restart";
        restartButton->Location = Point((overlayPanel->Width - 120) / 20, overlayPanel->Height / 2 - 5);
        restartButton->Size = System::Drawing::Size(175, 50);
        restartButton->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 14, System::Drawing::FontStyle::Bold)); // Increase font size
        restartButton->Click += gcnew EventHandler(this, &TetrisForm::OnRestartButtonClick);
        overlayPanel->Controls->Add(restartButton);

        // Initialize Main Menu Button
        mainMenuButton = gcnew Button();
        mainMenuButton->Text = "Main Menu";
        mainMenuButton->Location = Point((overlayPanel->Width - 210), overlayPanel->Height / 2 - 5);
        mainMenuButton->Size = System::Drawing::Size(200, 50);
        mainMenuButton->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 12, System::Drawing::FontStyle::Bold)); // Increase font size
        mainMenuButton->Click += gcnew EventHandler(this, &TetrisForm::OnMainMenuButtonClick);
        overlayPanel->Controls->Add(mainMenuButton);

        restartButton->Paint += gcnew PaintEventHandler(this, &TetrisForm::OnButtonPaint);
        mainMenuButton->Paint += gcnew PaintEventHandler(this, &TetrisForm::OnButtonPaint);

        /*-------------------------------------------------------------------------
                                    Score Board
        -------------------------------------------------------------------------*/
        Label^ scoreHeaderLabel = gcnew Label();
        scoreHeaderLabel->Text = "Score";
        scoreHeaderLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        scoreHeaderLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 10);
        scoreHeaderLabel->AutoSize = true;
        scoreHeaderLabel->BackColor = Color::LightGray;
        this->Controls->Add(scoreHeaderLabel);

        scoreValueLabel = gcnew Label();
        scoreValueLabel->Text = "0"; // Initial score value
        scoreValueLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16);
        scoreValueLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 40); // Set position
        scoreValueLabel->AutoSize = true; // Adjust size automatically
        scoreValueLabel->BackColor = Color::LightGray;
        this->Controls->Add(scoreValueLabel);

        // Create a panel for the score box
        Panel^ scorePanel = gcnew Panel();
        scorePanel->Location = System::Drawing::Point(outerPanel->Left - 350, outerPanel->Top); // Position it around the score value
        scorePanel->Size = System::Drawing::Size(200, 310); // Set size of the panel
        scorePanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle; // Add border
        scorePanel->BackColor = Color::LightGray; // Background color for visibility
        this->Controls->Add(scorePanel);

        // Level Header
        Label^ levelHeaderLabel = gcnew Label();
        levelHeaderLabel->Text = "Level";
        levelHeaderLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        levelHeaderLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 100); // Adjust position
        levelHeaderLabel->AutoSize = true;
        levelHeaderLabel->BackColor = Color::LightGray;
        this->Controls->Add(levelHeaderLabel);

        // Level Value
        levelValueLabel = gcnew Label();
        levelValueLabel->Text = "0"; // Initial level value
        levelValueLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16);
        levelValueLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 140); // Adjust position
        levelValueLabel->AutoSize = true; // Adjust size automatically
        levelValueLabel->BackColor = Color::LightGray;
        this->Controls->Add(levelValueLabel);

        // Lines Cleared Header
        Label^ linesHeaderLabel = gcnew Label();
        linesHeaderLabel->Text = "Lines";
        linesHeaderLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        linesHeaderLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 200); // Adjust position
        linesHeaderLabel->AutoSize = true;
        linesHeaderLabel->BackColor = Color::LightGray;
        this->Controls->Add(linesHeaderLabel);

        Label^ linesHeaderLabel2 = gcnew Label();
        linesHeaderLabel2->Text = "Cleared";
        linesHeaderLabel2->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        linesHeaderLabel2->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 230); // Adjust position
        linesHeaderLabel2->AutoSize = true;
        linesHeaderLabel2->BackColor = Color::LightGray;
        this->Controls->Add(linesHeaderLabel2);

        // Lines Cleared Value
        linesValueLabel = gcnew Label();
        linesValueLabel->Text = "0"; // Initial lines cleared value
        linesValueLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16);
        linesValueLabel->Location = System::Drawing::Point(outerPanel->Left - 340, outerPanel->Top + 265); // Adjust position
        linesValueLabel->AutoSize = true; // Adjust size automatically
        linesValueLabel->BackColor = Color::LightGray;
        this->Controls->Add(linesValueLabel);

        // Add score panel to the form
        this->Controls->Add(scorePanel);

        /*-------------------------------------------------------------------------
                                Hold and Next Shapes
        -------------------------------------------------------------------------*/

        // Initialize the hold shape panel
        holdPanel = gcnew Panel();
        holdPanel->Size = System::Drawing::Size(125, 100); // Set size for hold panel
        holdPanel->Location = System::Drawing::Point(outerPanel->Left - 135, outerPanel->Top + 25); // Position it
        holdPanel->BackColor = Color::LightGray; // Background color for visibility
        this->Controls->Add(holdPanel);

        // Lines Cleared Value
        holdLabel = gcnew Label();
        holdLabel->Text = "Hold"; // Initial lines cleared value
        holdLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16);
        holdLabel->Location = System::Drawing::Point(holdPanel->Left + (holdPanel->Width - holdLabel->Width) / 2, holdPanel->Top - holdLabel->Height); // Adjust position
        holdLabel->AutoSize = true; // Adjust size automatically
        holdLabel->BackColor = Color::LightGray;
        this->Controls->Add(holdLabel);

        // Initialize the next shape panel
        nextPanel = gcnew Panel();
        nextPanel->Size = System::Drawing::Size(125, 100); // Set size for next panel
        nextPanel->Location = System::Drawing::Point(outerPanel->Right + 10, outerPanel->Top + 25); // Position it
        nextPanel->BackColor = Color::LightGray; // Background color for visibility
        this->Controls->Add(nextPanel);

        nextLabel = gcnew Label();
        nextLabel->Text = "Next"; // Initial lines cleared value
        nextLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16);
        nextLabel->Location = System::Drawing::Point(nextPanel->Left + (nextPanel->Width - nextLabel->Width) / 2, nextPanel->Top - nextLabel->Height); // Adjust position
        nextLabel->AutoSize = true; // Adjust size automatically
        nextLabel->BackColor = Color::LightGray;
        this->Controls->Add(nextLabel);

        // Create a PictureBox for the holdPanel
        squarePicture = gcnew PictureBox();
        squarePicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        squarePicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        squarePicture->BackColor = Color::Black; // Set background color (adjust as needed)
        squarePicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        squarePicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        squareImagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\square.png");
        squarePicture->Image = Image::FromFile(squareImagePath);

        linePicture = gcnew PictureBox();
        linePicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        linePicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        linePicture->BackColor = Color::Black; // Set background color (adjust as needed)
        linePicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        linePicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        lineImagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\line.png");
        linePicture->Image = Image::FromFile(lineImagePath);

        tPicture = gcnew PictureBox();
        tPicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        tPicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        tPicture->BackColor = Color::Black; // Set background color (adjust as needed)
        tPicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        tPicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        timagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\t_shape.png");
        tPicture->Image = Image::FromFile(timagePath);

        sPicture = gcnew PictureBox();
        sPicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        sPicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        sPicture->BackColor = Color::Black; // Set background color (adjust as needed)
        sPicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        sPicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        simagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\s_shape.png");
        sPicture->Image = Image::FromFile(simagePath);

        zPicture = gcnew PictureBox();
        zPicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        zPicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        zPicture->BackColor = Color::Black; // Set background color (adjust as needed)
        zPicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        zPicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        zimagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\z_shape.png");
        zPicture->Image = Image::FromFile(zimagePath);

        lPicture = gcnew PictureBox();
        lPicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        lPicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        lPicture->BackColor = Color::Black; // Set background color (adjust as needed)
        lPicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        lPicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        limagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\l_shape.png");
        lPicture->Image = Image::FromFile(limagePath);

        jPicture = gcnew PictureBox();
        jPicture->Size = System::Drawing::Size(holdPanel->Width - 10, holdPanel->Height - 10); // Slightly smaller than the panel
        jPicture->Location = System::Drawing::Point(5, 5); // Center it within the holdPanel
        jPicture->BackColor = Color::Black; // Set background color (adjust as needed)
        jPicture->BorderStyle = BorderStyle::FixedSingle; // Optional: Add a border for clarity
        jPicture->SizeMode = PictureBoxSizeMode::CenterImage; // Adjust image sizing mode (if displaying an image)
        jimagePath = System::IO::Path::Combine(Application::StartupPath, "Resources\\j_shape.png");
        jPicture->Image = Image::FromFile(jimagePath);
        
        /*-------------------------------------------------------------------------
                                    High Score
        -------------------------------------------------------------------------*/
        // High Score Panel
        highScorePanel = gcnew Panel();
        highScorePanel->Size = System::Drawing::Size(300, 225);
        highScorePanel->Location = System::Drawing::Point(outerPanel->Right + 10, outerPanel->Top + 180); // Position it
        highScorePanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle; // Add border
        highScorePanel->BackColor = Color::LightGray; // Background color for visibility
        this->Controls->Add(highScorePanel);

        Label^ HighScoreHeaderLabel = gcnew Label();
        HighScoreHeaderLabel->Text = "High Score";
        HighScoreHeaderLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 20, FontStyle::Bold);
        HighScoreHeaderLabel->AutoSize = true;
        HighScoreHeaderLabel->BackColor = Color::LightGray;
        this->Controls->Add(HighScoreHeaderLabel);
        HighScoreHeaderLabel->Location = System::Drawing::Point(highScorePanel->Left + (highScorePanel->Width - HighScoreHeaderLabel->Width) / 2, highScorePanel->Top + 10);
        HighScoreHeaderLabel->BringToFront(); // Ensure it's above the panel

        //Panel to appear when achive new high score
        NewHighScorePanel = gcnew Panel();
        NewHighScorePanel->Size = System::Drawing::Size(400, 350); // Full size
        NewHighScorePanel->Location = System::Drawing::Point(
            (this->ClientSize.Width - NewHighScorePanel->Width) / 2,
            (this->ClientSize.Height - NewHighScorePanel->Height) / 2
        );
        NewHighScorePanel->BackColor = Color::DarkGray;
        NewHighScorePanel->Visible = false; // Initially hidden
        // Add the overlay panel to the form
        this->Controls->Add(NewHighScorePanel);

        NewHighScoreHeaderLabel = gcnew Label();
        NewHighScoreHeaderLabel->Text = "NEW HIGH \nSCORE!";
        NewHighScoreHeaderLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 30, FontStyle::Bold);
        NewHighScoreHeaderLabel->AutoSize = true;
        NewHighScoreHeaderLabel->BackColor = Color::DarkGray;
        NewHighScoreHeaderLabel->ForeColor = Color::Black; // Game over text color
        NewHighScoreHeaderLabel->TextAlign = ContentAlignment::MiddleCenter;
        NewHighScorePanel->Controls->Add(NewHighScoreHeaderLabel);
        NewHighScoreHeaderLabel->Location = System::Drawing::Point((NewHighScorePanel->Width - NewHighScoreHeaderLabel->Width) / 2, 10);
        NewHighScoreHeaderLabel->Visible = true;
        NewHighScoreHeaderLabel->BringToFront(); // Ensure it's above the panel

        EnterInitalsLabel = gcnew Label();
        EnterInitalsLabel->Text = "Enter Initals: ";
        EnterInitalsLabel->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        EnterInitalsLabel->AutoSize = true;
        EnterInitalsLabel->BackColor = Color::DarkGray;
        EnterInitalsLabel->ForeColor = Color::Black; // Game over text color
        EnterInitalsLabel->TextAlign = ContentAlignment::MiddleCenter;
        NewHighScorePanel->Controls->Add(EnterInitalsLabel);
        EnterInitalsLabel->Location = System::Drawing::Point(10, NewHighScoreHeaderLabel->Height + 25);
        EnterInitalsLabel->Visible = true;
        EnterInitalsLabel->BringToFront(); // Ensure it's above the panel

        EnterInitals = gcnew TextBox();
        EnterInitals->MaxLength = 3; // Limit to 3 characters
        EnterInitals->Font = gcnew System::Drawing::Font("Press Start 2P", 14);
        EnterInitals->CharacterCasing = CharacterCasing::Upper; // Force uppercase
        EnterInitals->Size = System::Drawing::Size(EnterInitalsLabel->Width, 50);
        EnterInitals->Location = System::Drawing::Point(15, EnterInitalsLabel->Bottom + 10);
        NewHighScorePanel->Controls->Add(EnterInitals);

        DisplayHighScoreHeader = gcnew Label();
        DisplayHighScoreHeader->Text = "Your Score: ";
        DisplayHighScoreHeader->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        DisplayHighScoreHeader->AutoSize = true;
        DisplayHighScoreHeader->BackColor = Color::DarkGray;
        DisplayHighScoreHeader->ForeColor = Color::Black; // Game over text color
        DisplayHighScoreHeader->TextAlign = ContentAlignment::MiddleCenter;
        NewHighScorePanel->Controls->Add(DisplayHighScoreHeader);
        DisplayHighScoreHeader->Location = System::Drawing::Point(10, EnterInitals->Bottom + 25);
        DisplayHighScoreHeader->Visible = true;
        DisplayHighScoreHeader->BringToFront(); // Ensure it's above the panel

        DisplayHighScore = gcnew Label();
        //DisplayHighScore->Text = gameBoard->Score;
        DisplayHighScore->Font = gcnew System::Drawing::Font("Press Start 2P", 16, FontStyle::Bold);
        DisplayHighScore->AutoSize = true;
        DisplayHighScore->BackColor = Color::DarkGray;
        DisplayHighScore->ForeColor = Color::Black; // Game over text color
        DisplayHighScore->TextAlign = ContentAlignment::MiddleCenter;
        NewHighScorePanel->Controls->Add(DisplayHighScore);
        DisplayHighScore->Location = System::Drawing::Point(10, DisplayHighScoreHeader->Bottom + 25);
        DisplayHighScore->Visible = true;
        DisplayHighScore->BringToFront(); // Ensure it's above the panel

        // Initialize New High Score Cancel Button
        NewHighScoreCancelButton = gcnew Button();
        NewHighScoreCancelButton->Text = "Cancel";
        NewHighScoreCancelButton->Location = Point(20, DisplayHighScore->Bottom + 20);
        NewHighScoreCancelButton->Size = System::Drawing::Size((NewHighScorePanel->Width / 2) - 30, 50);
        NewHighScoreCancelButton->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 12, System::Drawing::FontStyle::Bold)); // Increase font size
        NewHighScoreCancelButton->Click += gcnew EventHandler(this, &TetrisForm::OnHighScoreCancelButtonClick);
        NewHighScorePanel->Controls->Add(NewHighScoreCancelButton);
        NewHighScoreCancelButton->Paint += gcnew PaintEventHandler(this, &TetrisForm::OnButtonPaint);
        
        // Initialize New High Score Button
        NewHighScoreButton = gcnew Button();
        NewHighScoreButton->Text = "Submit";
        NewHighScoreButton->Location = Point(((NewHighScorePanel->Width - NewHighScoreButton->Width) / 2)+40, DisplayHighScore->Bottom + 20);
        NewHighScoreButton->Size = System::Drawing::Size((NewHighScorePanel->Width / 2) - 30, 50);
        NewHighScoreButton->Font = (gcnew System::Drawing::Font(L"Press Start 2P", 12, System::Drawing::FontStyle::Bold)); // Increase font size
        NewHighScoreButton->Click += gcnew EventHandler(this, &TetrisForm::OnHighScoreButtonClick);
        NewHighScorePanel->Controls->Add(NewHighScoreButton);
        NewHighScoreButton->Paint += gcnew PaintEventHandler(this, &TetrisForm::OnButtonPaint);

        


        /*-------------------------------------------------------------------------
                             Actually Create the Game Board
        -------------------------------------------------------------------------*/

        array<array<Point>^>^ InitializeTShape();
        array<array<Point>^>^ InitializeSquareShape();
        array<array<Point>^>^ InitializeLineShape();
        array<array<Point>^>^ InitializeSShape();
        array<array<Point>^>^ InitializeZShape();
        array<array<Point>^>^ InitializeLShape();
        array<array<Point>^>^ InitializeJShape();

        rand = gcnew Random(); // Initialize Random

        // Initialize variables
        hasHeldPiece = false;
        holdShapeType = 0; // No piece held initially
        nextShapeType = rand->Next(1, 8); // Random next shape
        nextShapeColor = GetShapeColor(nextShapeType); // Get the next shape's color
        holdPosition = Point(200, 200); // Default position
        SpawnNewShape(); // Spawn the first shape
        LoadHighScores();

    }

protected:
    /*-------------------------------------------------------------------------
                                Draw Functions
    -------------------------------------------------------------------------*/
    void DrawGrid(Object^ sender, PaintEventArgs^ e) {
        Graphics^ g = e->Graphics;

        // Draw the currently falling shape
        DrawShape(g, shapePosition, shapeColor); // Normal shape

        // Draw the ghost shape
        Point ghostPosition = CalculateGhostPosition();
        DrawGhostShape(g, ghostPosition, shapeColor); // Ghost shape with transparency


        // Draw the grid on top
        DrawGridLines(g);

        // Draw the pieces already placed on the game board
        gameBoard->Draw(g);

    }

    void DrawShape(Graphics^ g, Point position, Color color) {
        //Creaate a brush with the shapes passed in color
        SolidBrush^ brush = gcnew SolidBrush(color);

        //Create a shape for each of the points in the array
        for each (Point p in tShapeCoords[currentRotation]) {
            Rectangle rect = Rectangle(
                (position.X + p.X) * blockSize,
                (position.Y + p.Y) * blockSize,
                blockSize, blockSize
            );
            g->FillRectangle(brush, rect);
        }

        delete brush; // Clean up the brush
    }

    void DrawGhostShape(Graphics^ g, Point position, Color color) {
        // Create a semi-transparent brush for the ghost shape
        Color transparentColor = Color::FromArgb(128, color.R, color.G, color.B);
        SolidBrush^ brush = gcnew SolidBrush(transparentColor);

        for each (Point p in tShapeCoords[currentRotation]) {
            Rectangle rect = Rectangle(
                (position.X + p.X) * blockSize,
                (position.Y + p.Y) * blockSize,
                blockSize, blockSize

            );

            g->FillRectangle(brush, rect);
        }

        delete brush; // Clean up the brush
    }

    void DrawGridLines(Graphics^ g) {
        for (int i = 0; i <= cols; ++i) {
            g->DrawLine(Pens::Black, i * blockSize, 0, i * blockSize, rows * blockSize);
        }
        for (int j = 0; j <= rows; ++j) {
            g->DrawLine(Pens::Black, 0, j * blockSize, cols * blockSize, j * blockSize);
        }
    }
     
    void DrawShapeGrid(Graphics^ g, Rectangle shape) {
        // Draw grid inside the shape
        int gridSize = 25; // Size of each grid square
        Pen^ gridPen = Pens::Black;

        // Draw vertical grid lines
        for (int x = shape.X; x <= shape.X + shape.Width; x += gridSize) {
            g->DrawLine(gridPen, x, shape.Y, x, shape.Y + shape.Height);
        }

        // Draw horizontal grid lines
        for (int y = shape.Y; y <= shape.Y + shape.Height; y += gridSize) {
            g->DrawLine(gridPen, shape.X, y, shape.X + shape.Width, y);
        }
    }

    virtual void OnPaint(PaintEventArgs^ e) override {
        Graphics^ g = e->Graphics;
        e->Graphics->DrawImage(backgroundImage, 0, 0, this->ClientSize.Width, this->ClientSize.Height);

        // Only draw the game board if the game is not over
        if (!isGameOver) {
            gameBoard->Draw(g);
        }

        if (holdShapeType != 0) {
            DrawHoldShape();
        }
        //DrawHoldAndNextShapes(g);

        // Draw thick borders around the outer panel
        g->FillRectangle(Brushes::DimGray, outerPanel->Location.X, outerPanel->Location.Y + outerPanel->Height - borderThickness, outerPanel->Width, borderThickness); // Bottom border
        g->FillRectangle(Brushes::DimGray, outerPanel->Location.X, outerPanel->Location.Y, borderThickness, outerPanel->Height); // Left border
        g->FillRectangle(Brushes::DimGray, outerPanel->Location.X + outerPanel->Width - borderThickness, outerPanel->Location.Y, borderThickness, outerPanel->Height); // Right border

        // Draw overlay if game is over
        if (isGameOver) {
            //overlayPanel->Visible = true; // Ensure overlay is visible
            //overlayPanel->BringToFront(); // Bring overlay to the front
            //NewHighScorePanel->Visible = true; // Ensure overlay is visible
            //NewHighScorePanel->BringToFront(); // Bring overlay to the front
        }
    }

    /*-------------------------------------------------------------------------
                         Hold and Next Shape Fuctions
    -------------------------------------------------------------------------*/
    void DrawHoldShape() {
        if (holdShapeType != 0) {
            holdPanel->Controls->Clear();

            // Initialize the shape coordinates based on the holdShapeType
            switch (holdShapeType) {
            case 1: holdPanel->Controls->Add(tPicture); break;
            case 2: holdPanel->Controls->Add(squarePicture); break;
            case 3: holdPanel->Controls->Add(linePicture); break;
            case 4: holdPanel->Controls->Add(sPicture); break;
            case 5: holdPanel->Controls->Add(zPicture); break;
            case 6: holdPanel->Controls->Add(lPicture); break;
            case 7: holdPanel->Controls->Add(jPicture); break;
            }

        }
    }

    void DrawNextShape() {
        if (nextShapeType != 0) {
            nextPanel->Controls->Clear();
            // Initialize the shape coordinates based on the nextShapeType
            switch (nextShapeType) {
            case 1: nextPanel->Controls->Add(tPicture); break;
            case 2: nextPanel->Controls->Add(squarePicture); break;
            case 3: nextPanel->Controls->Add(linePicture); break;
            case 4: nextPanel->Controls->Add(sPicture); break;
            case 5: nextPanel->Controls->Add(zPicture); break;
            case 6: nextPanel->Controls->Add(lPicture); break;
            case 7: nextPanel->Controls->Add(jPicture); break;
            }
        }
    }

    void OnHoldKeyPressed(EventArgs^ e) {
        if (isGameOver) return;

        if (!hasHeldPiece) {
            // If no piece has been held yet, store the current piece in hold
            holdShapeType = shapeType;
            holdShapeColor = shapeColor;
            DrawHoldShape();
            //holdPosition = shapePosition;

            // Spawn a new shape after the hold
            SpawnNewShape();

            hasHeldPiece = true; // Mark the piece as held
        }
        else {
            // Temporarily store current shape properties
            int tempShapeType = shapeType;
            Color tempShapeColor = shapeColor;
            int tempRotation = currentRotation;

            // Swap the held piece with the current piece
            shapeType = holdShapeType;
            shapeColor = holdShapeColor;
            currentRotation = 0; // Reset rotation of the held piece when it is brought into play

            holdShapeType = tempShapeType;
            holdShapeColor = tempShapeColor;
            DrawHoldShape();
            switch (shapeType) {
            case 1: tShapeCoords = InitializeTShape(); break;
            case 2: tShapeCoords = InitializeSquareShape(); break;
            case 3: tShapeCoords = InitializeLineShape(); break;
            case 4: tShapeCoords = InitializeSShape(); break;
            case 5: tShapeCoords = InitializeZShape(); break;
            case 6: tShapeCoords = InitializeLShape(); break;
            case 7: tShapeCoords = InitializeJShape(); break;
            }
            // Position the new shape at the starting position (top center of the board)
        }

    }

    /*-------------------------------------------------------------------------
                           Button Functions
    -------------------------------------------------------------------------*/
    void OnRestartButtonClick(Object^ sender, EventArgs^ e) {
        gameBoard->Reset(); // Reset the game board
        UpdateScore(gameBoard->Score);
        UpdateLevel(gameBoard->Level);
        UpdateLines(gameBoard->LinesCleared);

        // Clear the game board and reset the game
        gameBoard->Clear(); // Make sure you implement the Clear method in your GameBoard class
        isGameOver = false; // Reset game over flag
        gameOverLabel->Visible = false; // Hide the game over label
        overlayPanel->Visible = false; // Hide the overlay
        fallInterval = 1000;
        holdPanel->Controls->Clear();
        nextPanel->Controls->Clear();
        holdShapeType = 0;

        // Reset shape-related variables
        shapePosition = Point(cols / 2, 0); // Reset shape position
        currentRotation = 0; // Reset rotation
        SpawnNewShape(); // Spawn the first shape
        fallTimer->Interval = fallInterval;
        fallTimer->Start(); // Restart the timer
        boardPanel->Invalidate(); // Redraw the board
        // Ensure the form regains focus for key events
        this->Focus();
    }

    void OnMainMenuButtonClick(Object^ sender, EventArgs^ e) {
        this->Close(); // Close the form and return to the main menu
    }

    void OnHighScoreCancelButtonClick(Object^ sender, EventArgs^ e) {
        gameOverLabel->Visible = true; // Show game over label
        overlayPanel->Visible = true; // Show the overlay panel
        restartButton->Visible = true;
        mainMenuButton->Visible = true;
        NewHighScorePanel->Visible = false; // Ensure overlay is visible
        NewHighScoreHeaderLabel->Visible = false;
        restartButton->Focus();
        overlayPanel->BringToFront();
    }

    void OnHighScoreButtonClick(Object^ sender, EventArgs^ e) {
        // Assuming you already have the connection object set up
        SqlConnection^ conn = gcnew SqlConnection(connectionString);

        try {
            // Open the connection
            conn->Open();

            if (EnterInitals->Text != "" && DisplayHighScore->Text != "") {
                String^ query = "INSERT INTO TETRIS_HIGH_SCORES (INITAL, SCORE) VALUES (@name, @score)";
                SqlCommand^ command = gcnew SqlCommand(query, conn);
                command->Parameters->AddWithValue("@name", EnterInitals->Text);
                command->Parameters->AddWithValue("@score", Convert::ToInt32(DisplayHighScore->Text)); // Ensure score is stored as an integer

                // Execute command
                int rowsAffected = command->ExecuteNonQuery();
                LoadHighScores();
                EnterInitals->Text = "";
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error: " + ex->Message);
        }
        finally {
            // Close the connection
            if (conn->State == ConnectionState::Open) {
                conn->Close();
            }
        }

        gameOverLabel->Visible = true; // Show game over label
        overlayPanel->Visible = true; // Show the overlay panel
        restartButton->Visible = true;
        mainMenuButton->Visible = true;
        NewHighScorePanel->Visible = false; // Ensure overlay is visible
        NewHighScoreHeaderLabel->Visible = false;
        restartButton->Focus();
        overlayPanel->BringToFront();
    }

    void OnFallTimerTick(Object^ sender, EventArgs^ e) {
        if (isGameOver) return; // Prevent further processing if game is over

        // Check if the shape can move down
        if (gameBoard->CanPlacePiece(shapePosition.X, shapePosition.Y + 1, tShapeCoords[currentRotation])) {
            shapePosition.Y += 1; // Move down
        }
        else {
            // Place the piece on the board
            gameBoard->PlacePiece(shapePosition.X, shapePosition.Y, tShapeCoords[currentRotation], shapeType);
            // Update labels to reflect current game state
            UpdateScore(gameBoard->Score);
            UpdateLevel(gameBoard->Level);
            UpdateLines(gameBoard->LinesCleared);

            // Check for game over conditions
            for each (Point p in tShapeCoords[currentRotation]) {
                int x = shapePosition.X + p.X;
                int y = shapePosition.Y + p.Y;
                if (y < 0 || (gameBoard->IsOccupied(x, y) && y < 1)) {
                    isGameOver = true;
                    fallTimer->Stop(); // Stop the fall timer
                    CheckNewHighScore();
                    break; // Exit the loop early
                }
            }
            SpawnNewShape(); // Spawn a new shape
        }

        // Invalidate the board panel to trigger a repaint
        boardPanel->Invalidate();
    }

    /*-------------------------------------------------------------------------
                              Database Functions
    -------------------------------------------------------------------------*/
    void LoadHighScores() {
        try {
            SqlConnection^ conn = gcnew SqlConnection(connectionString);
            conn->Open();
            String^ query = "SELECT TOP 5 INITAL, SCORE FROM TETRIS_HIGH_SCORES ORDER BY SCORE DESC";
            SqlCommand^ cmd = gcnew SqlCommand(query, conn);
            SqlDataReader^ reader = cmd->ExecuteReader();

            int y = 50;
            //int y = highScorePanel->Top + highScoreHeaderLabel->Height + 10;
            int count = 1;
            highScorePanel->Controls->Clear(); // Clear previous labels

            while (reader->Read()) {
                String^ playerName = reader["INITAL"]->ToString();
                int score = Convert::ToInt32(reader["SCORE"]);

                Label^ lbl = gcnew Label();
                lbl->Text = count + " " + playerName + " " + score.ToString() + "pts";
                lbl->Location = System::Drawing::Point(10, y);
                lbl->AutoSize = true;
                lbl->Font = gcnew System::Drawing::Font("Press Start 2P", 14);

                highScorePanel->Controls->Add(lbl);
                y += 35;
                count += 1;
            }

            reader->Close();
            conn->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error loading high scores: " + ex->Message);
        }
    }

    void CheckNewHighScore(){
        // Assuming you already have the connection object set up
        SqlConnection^ conn = gcnew SqlConnection(connectionString);

        try {
            // Open the connection
            conn->Open();

            // Query to get the top 5 scores
            String^ query = "SELECT TOP 5 INITAL, SCORE FROM TETRIS_HIGH_SCORES ORDER BY SCORE DESC";

            // Create the SQL command
            SqlCommand^ cmd = gcnew SqlCommand(query, conn);

            // Execute the command and get the data
            SqlDataReader^ reader = cmd->ExecuteReader();

            // Check if there are at least 5 scores
            if (reader->HasRows) {
                int lowestTopScore = 0;
                int count = 0;

                // Loop through the top 5 scores
                while (reader->Read()) {
                    int score = Convert::ToInt32(reader["SCORE"]);
                    if (count == 4) {
                        lowestTopScore = score; // Store the lowest score in the top 5
                    }
                    count++;
                }

                // Check if the player's score qualifies (is higher than the lowest score)
                if (gameBoard->Score > lowestTopScore) {
                    gameOverLabel->Visible = false; // Show game over label
                    overlayPanel->Visible = false; // Show the overlay panel
                    restartButton->Visible = false;
                    mainMenuButton->Visible = false;
                    // Show a panel if the player's score is in the top 5
                    DisplayHighScore->Text = gameBoard->Score.ToString();
                    NewHighScorePanel->Visible = true; // Ensure overlay is visible
                    NewHighScoreHeaderLabel->Visible = true;
                    EnterInitals->Focus();
                    NewHighScorePanel->BringToFront(); // Bring overlay to the front
                    
                }
                else {

                    gameOverLabel->Visible = true; // Show game over label
                    overlayPanel->Visible = true; // Show the overlay panel
                    overlayPanel->BringToFront(); // Bring overlay to the front

                    // Set focus to the restart button
                    restartButton->Focus(); // Focus on the restart button
                }
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error: " + ex->Message);
        }
        finally {
            // Close the connection
            if (conn->State == ConnectionState::Open) {
                conn->Close();
            }
        }

    }


    /*-------------------------------------------------------------------------
                                    Key Press
    -------------------------------------------------------------------------*/
    virtual void OnKeyDown(KeyEventArgs^ e) override {
        switch (e->KeyCode) {
        case Keys::Down:
            if (!isGameOver) {
                if (gameBoard->CanPlacePiece(shapePosition.X, shapePosition.Y + 1, tShapeCoords[currentRotation])) {
                    shapePosition.Y += 1; // Move down
                }
                else {
                    gameBoard->PlacePiece(shapePosition.X, shapePosition.Y, tShapeCoords[currentRotation], shapeType);
                    SpawnNewShape();
                    // Update labels to reflect current game state
                    UpdateScore(gameBoard->Score);
                    UpdateLevel(gameBoard->Level);
                    UpdateLines(gameBoard->LinesCleared);

                }
            }
            break;
        case Keys::Left:
            if (!isGameOver) {
                if (gameBoard->CanPlacePiece(shapePosition.X - 1, shapePosition.Y, tShapeCoords[currentRotation])) {
                    shapePosition.X -= 1; // Move left
                }
            }
            break;
        case Keys::Right:
            if (!isGameOver) {
                if (gameBoard->CanPlacePiece(shapePosition.X + 1, shapePosition.Y, tShapeCoords[currentRotation])) {
                    shapePosition.X += 1; // Move right
                }
            }
            break;
        case Keys::Up:
            if (!isGameOver) {
                RotateShape(); // Rotate shape logic here
            }
            break;
        case Keys::Space:
            if (!isGameOver) {
                while (gameBoard->CanPlacePiece(shapePosition.X, shapePosition.Y + 1, tShapeCoords[currentRotation])) {
                    shapePosition.Y += 1; // Move down until it can't
                }
                // Once it can't move down anymore, place the shape
                gameBoard->PlacePiece(shapePosition.X, shapePosition.Y, tShapeCoords[currentRotation], shapeType);
                SpawnNewShape(); // Spawn a new shape
                // Update labels to reflect current game state
                UpdateScore(gameBoard->Score);
                UpdateLevel(gameBoard->Level);
                UpdateLines(gameBoard->LinesCleared);

            }
            break;
        case Keys::C: // "Hold" key
            OnHoldKeyPressed(e); // Handle the hold functionality
            break;
        case Keys::Escape:
            this->Close(); // Close the form
            break;
        }

        // Invalidate the board panel to redraw
        boardPanel->Invalidate();
        Form::OnKeyDown(e);
    }
};