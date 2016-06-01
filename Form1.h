/*
3-clause BSD license

Copyright (c) 2014-2016 Michael Krause (krause@tum.de) Institute of Ergonomics, Technische Universität München

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/
#pragma once

#include <windows.h> 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h> 
#include <wchar.h> 
#include <string.h> 
#include <winnls.h>
#include <vcclr.h> // required header for PtrToStringChars
#include <msclr/marshal.h> 
#include <msclr\marshal_cppstd.h> 


#pragma unmanaged
	typedef void (__cdecl *MCB)(int, int, int, int, void*);//MouseCallBack

	typedef struct{
			int p[8];
	}  ints;

	const int TEMPLATE_COUNT = 20;//generate 20 scaled templates from the original template
	IplImage* templateFiles[TEMPLATE_COUNT];//store the 20 templates to save time

#pragma managed



namespace show {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace msclr::interop; 
	using namespace System::Runtime::InteropServices;//alloc
	using namespace System::Configuration;// setting reader 
	using namespace System::Threading;


	/// <summary>
	/// Zusammenfassung für Form1
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie auch
	///          die Ressourcendateiname-Eigenschaft für das Tool zur Kompilierung verwalteter Ressourcen ändern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abhängt.
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate void mouseCallbackDelegate(int, int, int, int, void*);
	public:
	mouseCallbackDelegate^ fpFrame;
	GCHandle gchFrame;
	IntPtr ipFrame;

	private: System::Windows::Forms::Button^  match;


	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Panel^  panel1;


	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;

	public: 
		static CvMouseCallback mcbFrameWin;
	private: System::Windows::Forms::Button^  acceptB;
	public: 


	private: System::Windows::Forms::Button^  aboutB;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::RadioButton^  invalidOnlyRadioButton;
	private: System::Windows::Forms::NumericUpDown^  thresholdUpDown;
	private: System::Windows::Forms::Label^  thresholdLabel3;
	private: System::Windows::Forms::Label^  thresholdLabel2;
	private: System::Windows::Forms::Label^  thresholdLabel;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::RadioButton^  graySkipRadioButton;


	private: System::Windows::Forms::RadioButton^  markInvalidRadioButton;
	private: System::Windows::Forms::Label^  navInfoLabel;

	private: System::Windows::Forms::RadioButton^  allRadioButton;













	public: Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//

			//msg callback for threads etc
			msgCallback = gcnew d_msgCallback(this, &show::Form1::updateUI); 


			//open openCV windows

			//frameWindow 'EYE'
				cvNamedWindow("frameWin", CV_WINDOW_AUTOSIZE);

				fpFrame = gcnew mouseCallbackDelegate(&show::Form1::myMouseHandlerFrame);
				//fp->Invoke(0,0,0,0,NULL);

				gchFrame = GCHandle::Alloc(fpFrame);
				ipFrame = Marshal::GetFunctionPointerForDelegate(fpFrame);
				//pin_ptr<mouseCallbackDelegate^> pp;

				mcbFrameWin = static_cast<MCB>(ipFrame.ToPointer());
				cvSetMouseCallback("frameWin", mcbFrameWin, NULL);


			//load template file
			templateFile = cvLoadImage("template.png", CV_LOAD_IMAGE_GRAYSCALE);

			if (templateFile){
				//MessageBox::Show( "OK " + path);
				//scale template
				for(int i=0; i< TEMPLATE_COUNT;i++){
					double scale = (1.0 / (double)TEMPLATE_COUNT) * ((double)i+3.0);
					int w = Math::Round(templateFile->width * scale);
					int h = Math::Round(templateFile->height * scale);
					templateFiles[i] = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
					cvResize(templateFile,templateFiles[i]);
					//cvShowImage("frameWin", templateFiles[i]); // display the frame
					//MessageBox::Show( w.ToString());
				}
			}else{
				MessageBox::Show( "Template file not found!");
				this->Close();
			}


		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}

			if(eyeFrame){
				pin_ptr<IplImage*> p;
				p = &eyeFrame;
				cvReleaseImage(p);
			}

			if(templateFile){
				pin_ptr<IplImage*> p;
				p = &templateFile;
				cvReleaseImage(p);
			}

			for(int i=0; i < TEMPLATE_COUNT;i++){
				if(templateFiles[i]){
					pin_ptr<IplImage*> p;
					p = &templateFiles[i];
					cvReleaseImage(p);
				}
			}


		}
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	protected: 
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  openB;
	private: System::Windows::Forms::Label^  fileLabel;
	private: System::Windows::Forms::TextBox^  openCVstatus;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  frameT;
	private: System::Windows::Forms::Button^  nextB;
	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openB = (gcnew System::Windows::Forms::Button());
			this->fileLabel = (gcnew System::Windows::Forms::Label());
			this->openCVstatus = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->frameT = (gcnew System::Windows::Forms::Label());
			this->nextB = (gcnew System::Windows::Forms::Button());
			this->match = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->navInfoLabel = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->graySkipRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->markInvalidRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->thresholdLabel3 = (gcnew System::Windows::Forms::Label());
			this->thresholdLabel2 = (gcnew System::Windows::Forms::Label());
			this->thresholdLabel = (gcnew System::Windows::Forms::Label());
			this->thresholdUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->invalidOnlyRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->allRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->aboutB = (gcnew System::Windows::Forms::Button());
			this->acceptB = (gcnew System::Windows::Forms::Button());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->panel1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->thresholdUpDown))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			this->dataGridView1->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::EnableWithoutHeaderText;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->EditMode = System::Windows::Forms::DataGridViewEditMode::EditOnEnter;
			this->dataGridView1->Location = System::Drawing::Point(12, 12);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(935, 229);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellEnter += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Form1::dataGridView1_CellEnter);
			this->dataGridView1->RowStateChanged += gcnew System::Windows::Forms::DataGridViewRowStateChangedEventHandler(this, &Form1::dataGridView1_RowStateChanged);
			this->dataGridView1->Enter += gcnew System::EventHandler(this, &Form1::dataGridView1_Enter);
			// 
			// openB
			// 
			this->openB->Location = System::Drawing::Point(6, 19);
			this->openB->Name = L"openB";
			this->openB->Size = System::Drawing::Size(93, 37);
			this->openB->TabIndex = 1;
			this->openB->Text = L"1. open journalfile";
			this->openB->UseVisualStyleBackColor = true;
			this->openB->Click += gcnew System::EventHandler(this, &Form1::openB_Click);
			// 
			// fileLabel
			// 
			this->fileLabel->AutoSize = true;
			this->fileLabel->Location = System::Drawing::Point(3, 3);
			this->fileLabel->Name = L"fileLabel";
			this->fileLabel->Size = System::Drawing::Size(16, 13);
			this->fileLabel->TabIndex = 2;
			this->fileLabel->Text = L"---";
			// 
			// openCVstatus
			// 
			this->openCVstatus->Location = System::Drawing::Point(9, 91);
			this->openCVstatus->Multiline = true;
			this->openCVstatus->Name = L"openCVstatus";
			this->openCVstatus->ReadOnly = true;
			this->openCVstatus->Size = System::Drawing::Size(492, 71);
			this->openCVstatus->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 80);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"status";
			// 
			// frameT
			// 
			this->frameT->AutoSize = true;
			this->frameT->Location = System::Drawing::Point(531, 97);
			this->frameT->Name = L"frameT";
			this->frameT->Size = System::Drawing::Size(36, 13);
			this->frameT->TabIndex = 2;
			this->frameT->Text = L"frame:";
			// 
			// nextB
			// 
			this->nextB->Location = System::Drawing::Point(532, 112);
			this->nextB->Name = L"nextB";
			this->nextB->Size = System::Drawing::Size(73, 37);
			this->nextB->TabIndex = 5;
			this->nextB->Text = L"show next frame >";
			this->nextB->UseVisualStyleBackColor = true;
			this->nextB->Click += gcnew System::EventHandler(this, &Form1::nextB_Click);
			// 
			// match
			// 
			this->match->Location = System::Drawing::Point(220, 19);
			this->match->Name = L"match";
			this->match->Size = System::Drawing::Size(102, 37);
			this->match->TabIndex = 7;
			this->match->Text = L"3. pupil detection";
			this->match->UseVisualStyleBackColor = true;
			this->match->Click += gcnew System::EventHandler(this, &Form1::match_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(830, 18);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(94, 37);
			this->button3->TabIndex = 7;
			this->button3->Text = L"7. save journalfile";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::saveButton_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->navInfoLabel);
			this->panel1->Controls->Add(this->groupBox2);
			this->panel1->Controls->Add(this->thresholdLabel3);
			this->panel1->Controls->Add(this->thresholdLabel2);
			this->panel1->Controls->Add(this->thresholdLabel);
			this->panel1->Controls->Add(this->thresholdUpDown);
			this->panel1->Controls->Add(this->groupBox1);
			this->panel1->Controls->Add(this->aboutB);
			this->panel1->Controls->Add(this->acceptB);
			this->panel1->Controls->Add(this->openB);
			this->panel1->Controls->Add(this->fileLabel);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->openCVstatus);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->frameT);
			this->panel1->Controls->Add(this->match);
			this->panel1->Controls->Add(this->nextB);
			this->panel1->Location = System::Drawing::Point(12, 244);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(935, 167);
			this->panel1->TabIndex = 13;
			// 
			// navInfoLabel
			// 
			this->navInfoLabel->AutoSize = true;
			this->navInfoLabel->Location = System::Drawing::Point(531, 151);
			this->navInfoLabel->Name = L"navInfoLabel";
			this->navInfoLabel->Size = System::Drawing::Size(356, 13);
			this->navInfoLabel->TabIndex = 32;
			this->navInfoLabel->Text = L"You can also use up/down arrow and page up/down when grid has focus";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->graySkipRadioButton);
			this->groupBox2->Controls->Add(this->markInvalidRadioButton);
			this->groupBox2->Location = System::Drawing::Point(553, 19);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(135, 56);
			this->groupBox2->TabIndex = 28;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"5. Gray row handling";
			// 
			// graySkipRadioButton
			// 
			this->graySkipRadioButton->AutoSize = true;
			this->graySkipRadioButton->Location = System::Drawing::Point(3, 33);
			this->graySkipRadioButton->Name = L"graySkipRadioButton";
			this->graySkipRadioButton->Size = System::Drawing::Size(75, 17);
			this->graySkipRadioButton->TabIndex = 1;
			this->graySkipRadioButton->Text = L"do nothing";
			this->graySkipRadioButton->UseVisualStyleBackColor = true;
			// 
			// markInvalidRadioButton
			// 
			this->markInvalidRadioButton->AutoSize = true;
			this->markInvalidRadioButton->Checked = true;
			this->markInvalidRadioButton->Location = System::Drawing::Point(3, 16);
			this->markInvalidRadioButton->Name = L"markInvalidRadioButton";
			this->markInvalidRadioButton->Size = System::Drawing::Size(131, 17);
			this->markInvalidRadioButton->TabIndex = 0;
			this->markInvalidRadioButton->TabStop = true;
			this->markInvalidRadioButton->Text = L"mark as invalid (blinks)";
			this->markInvalidRadioButton->UseVisualStyleBackColor = true;
			// 
			// thresholdLabel3
			// 
			this->thresholdLabel3->AutoSize = true;
			this->thresholdLabel3->Location = System::Drawing::Point(351, 69);
			this->thresholdLabel3->Name = L"thresholdLabel3";
			this->thresholdLabel3->Size = System::Drawing::Size(190, 13);
			this->thresholdLabel3->TabIndex = 31;
			this->thresholdLabel3->Text = L"gray (blinks); adjust until blinks are gray";
			// 
			// thresholdLabel2
			// 
			this->thresholdLabel2->AutoSize = true;
			this->thresholdLabel2->Location = System::Drawing::Point(351, 55);
			this->thresholdLabel2->Name = L"thresholdLabel2";
			this->thresholdLabel2->Size = System::Drawing::Size(171, 13);
			this->thresholdLabel2->TabIndex = 30;
			this->thresholdLabel2->Text = L"Frames with fit below this value are";
			// 
			// thresholdLabel
			// 
			this->thresholdLabel->AutoSize = true;
			this->thresholdLabel->Location = System::Drawing::Point(338, 42);
			this->thresholdLabel->Name = L"thresholdLabel";
			this->thresholdLabel->Size = System::Drawing::Size(124, 13);
			this->thresholdLabel->TabIndex = 29;
			this->thresholdLabel->Text = L"4. \'templateFit\' Threshold";
			// 
			// thresholdUpDown
			// 
			this->thresholdUpDown->Location = System::Drawing::Point(341, 21);
			this->thresholdUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->thresholdUpDown->Name = L"thresholdUpDown";
			this->thresholdUpDown->Size = System::Drawing::Size(57, 20);
			this->thresholdUpDown->TabIndex = 28;
			this->thresholdUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {650, 0, 0, 0});
			this->thresholdUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::thresholdUpDown_ValueChanged_1);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->invalidOnlyRadioButton);
			this->groupBox1->Controls->Add(this->allRadioButton);
			this->groupBox1->Location = System::Drawing::Point(110, 20);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(101, 57);
			this->groupBox1->TabIndex = 27;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"2. Which frames";
			// 
			// invalidOnlyRadioButton
			// 
			this->invalidOnlyRadioButton->AutoSize = true;
			this->invalidOnlyRadioButton->Checked = true;
			this->invalidOnlyRadioButton->Location = System::Drawing::Point(3, 16);
			this->invalidOnlyRadioButton->Name = L"invalidOnlyRadioButton";
			this->invalidOnlyRadioButton->Size = System::Drawing::Size(77, 17);
			this->invalidOnlyRadioButton->TabIndex = 1;
			this->invalidOnlyRadioButton->TabStop = true;
			this->invalidOnlyRadioButton->Text = L"invalid only";
			this->invalidOnlyRadioButton->UseVisualStyleBackColor = true;
			// 
			// allRadioButton
			// 
			this->allRadioButton->AutoSize = true;
			this->allRadioButton->Location = System::Drawing::Point(3, 33);
			this->allRadioButton->Name = L"allRadioButton";
			this->allRadioButton->Size = System::Drawing::Size(35, 17);
			this->allRadioButton->TabIndex = 0;
			this->allRadioButton->Text = L"all";
			this->allRadioButton->UseVisualStyleBackColor = true;
			// 
			// aboutB
			// 
			this->aboutB->Location = System::Drawing::Point(908, 143);
			this->aboutB->Name = L"aboutB";
			this->aboutB->Size = System::Drawing::Size(24, 21);
			this->aboutB->TabIndex = 24;
			this->aboutB->Text = L"\?";
			this->aboutB->UseVisualStyleBackColor = true;
			this->aboutB->Click += gcnew System::EventHandler(this, &Form1::button4_Click_1);
			// 
			// acceptB
			// 
			this->acceptB->Location = System::Drawing::Point(694, 18);
			this->acceptB->Name = L"acceptB";
			this->acceptB->Size = System::Drawing::Size(122, 37);
			this->acceptB->TabIndex = 22;
			this->acceptB->Text = L"6. accept green rows; handle gray rows";
			this->acceptB->UseVisualStyleBackColor = true;
			this->acceptB->Click += gcnew System::EventHandler(this, &Form1::acceptB_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(959, 433);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->dataGridView1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"Form1";
			this->Text = L"pupille 1.8";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->thresholdUpDown))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion


	array<String^>^ filePaths; // array of file names for batch processing
	static int filePathsIndex; //current index of batch processed file
	System::Boolean batch;//true if batch process is running

	static CvCapture* capture1;
	static IplImage *eyeFrame;
	static IplImage *templateFile;
	//static IplImage* result1;	
	static int currentFrame;
	System::Boolean static detecting;//flag; true if pupile detection is in progress

	//CvCapture* capture2;
	//IplImage *frame2;
	System::Boolean loading;//true if csv is actually loading
	String^ firstlineOfTXT;
	String^ secondlineOfTXT;

	//static IplImage* img1;
	int static ddx,ddy;
	int static drag;
	//boolean static secondVideo;
    //int secondVideoOffset;


#define MSG_SELECT_ROW 1
#define MSG_PROGRESS 2
#define MSG_FORM_OFF 3
#define MSG_FORM_ON 4
#define MSG_DONE 5
		 

	private: System::Void open(System::String^ fn){
				 //msgCallback = gcnew d_msgCallback(this, &show::Form1::updateUI); 
				 msgCallback(MSG_FORM_OFF, 0);//enable some user interaction with form

				 loading = true;

				String^ line;
				String^ delimStr = "\t"; //" ,.:\t";
				array<Char>^ delimiter = delimStr->ToCharArray( );
				array<String^>^ words;

				 int count = 0;
				 fileLabel->Text = fn ;
				 openCVstatus->Text = "";
					

				 dataGridView1->Rows->Clear();
				 dataGridView1->Columns->Clear();
				 dataGridView1->Rows->Clear();

				System::String^ journalFileName = fn;


				 
				 try{//read csv

				  //StreamReader^ din = File::OpenText(this->openFileDialog1->FileName);
				  StreamReader^ din = File::OpenText(journalFileName);

				  int i;

				  while ((line = din->ReadLine()) != nullptr){
					words = line->Split( delimiter );
					if (count == 0){
						firstlineOfTXT = line;
					}
					if (count == 1){
						secondlineOfTXT = line;

						for(i=0;i < words->Length; i++){
							int intFoo = -1;
							dataGridView1->Columns->Add(words[i], words[i]);

							if (words[i] != "event"){
								dataGridView1->Columns[words[i]]->ValueType = intFoo.GetType();
							}

							dataGridView1->Columns[words[i]]->ReadOnly = true;
						}

					    int intFoo = -1;
						dataGridView1->Columns["index"]->ValueType = intFoo.GetType();
						dataGridView1->Columns->Add("templateX", "templateX");//template was found at tempX tempY
						dataGridView1->Columns["templateX"]->ValueType = intFoo.GetType();
						dataGridView1->Columns->Add("templateY", "templateY");
						dataGridView1->Columns["templateY"]->ValueType = intFoo.GetType();
						dataGridView1->Columns->Add("templateFit", "templateFit");//template similarity; lower value is better
						dataGridView1->Columns["templateFit"]->ValueType = intFoo.GetType();
						dataGridView1->Columns->Add("aboveThres", "aboveThres");
						dataGridView1->Columns["aboveThres"]->ValueType = intFoo.GetType();

						//index is sometimes != frame (first frames sometimes have strange index e.g. 7531, etc)
						dataGridView1->Columns->Add("frame", "frame");//therefore 'frame' column is kind of unique ID 
						dataGridView1->Columns["frame"]->ValueType = intFoo.GetType();
						dataGridView1->Columns["frame"]->ReadOnly = true;

						dataGridView1->Columns[0]->Frozen = true;//index column always visible

					}

					if (count > 1){
						  //dataGridView1->Rows->Add(words);//insert everything as string
						  dataGridView1->Rows->Add();//insert empty row
						  
						  int r = dataGridView1->Rows->Count-1;//last row
						  dataGridView1->Rows[r]->Cells["frame"]->Value = r;

						  Int32 numValue;
						  for(i=0;i < words->Length; i++){//try to parse everything as int, if possible (trigger-events will fail)
							String^ name = dataGridView1->Columns[i]->Name;//get column name
							bool parsed = Int32::TryParse(words[i], numValue);//try to parse data from file
							if (parsed) dataGridView1->Rows[r]->Cells[name]->Value = numValue;//insert int into column
							else dataGridView1->Rows[r]->Cells[name]->Value = words[i];//insert string into column
						  }

						  
					}

					count++;
				  }//while

				 }catch(...){
					 openCVstatus->AppendText("Fehler beim Öffnen oder Lesen von Datei: " + this->openFileDialog1->FileName + " Zeile: " + count.ToString());
				 }
				 




				 try{//open videofiles


					 
					 //System::String^ temp = this->openFileDialog1->FileName->Replace("journal","eye");
					 System::String^ eyeVideoName = journalFileName->Replace("journal","eye");
					 eyeVideoName = eyeVideoName->Replace(".txt","-0000.avi");


					 String^ dirStr = "\\"; //" ,.:\t";
					 array<Char>^ dirdelimiter = dirStr->ToCharArray( );
					 array<String^>^ dirs;
					 dirs = eyeVideoName->Split( dirdelimiter );
					 eyeVideoName = dirs[0];
					 for(int i=1;i < dirs->Length; i++){
						 if (i == (dirs->Length-2)){
							 eyeVideoName = eyeVideoName +  "\\" + "online";
						 }else{
							eyeVideoName = eyeVideoName + "\\" + dirs[i];
						 }
					 }

					 

					 if(capture1){ 
						pin_ptr<CvCapture*> p;
						p = &capture1;
						cvReleaseCapture( p ); 
					 }//release old one before create new


					 //System::String^ videofile1 = this->openFileDialog1->FileName->Replace("_data.csv","_1.mpeg");
					 const char* filename1 = (char*)(void*)Marshal::StringToHGlobalAnsi(eyeVideoName);
					 if (!(capture1 = cvCreateFileCapture(filename1))){
						 openCVstatus->AppendText("Error opening: " +eyeVideoName+ "\r\n");
					 }else{
						 //cvSetCaptureProperty(capture1, CV_CAP_PROP_FOURCC, CV_FOURCC('P','I','M','1') );
						int no_of_frames = (int) cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
						openCVstatus->AppendText("Frames: " +no_of_frames.ToString());
						


						show(0);//firstframe
					 }

				 }
				 catch(...){
				 }

			   loading = false;
			   msgCallback(MSG_FORM_ON, 0);//enable some user interaction with form
			 
			 }

	private: System::Void openB_Click(System::Object^  sender, System::EventArgs^  e) {
 

			//this->openFileDialog1->InitialDirectory = "."; 

             this->openFileDialog1->Filter = "Textdateien (*.txt)|*.txt"; 
             this->openFileDialog1->Multiselect = false; 

             this->openFileDialog1->FileName = ""; 


             if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK){
				 open(openFileDialog1->FileName);
             } 

			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {


			 }
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

		if (MessageBox::Show("Are you sure you want to quit? (Did you save your results?)", "Exit", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult ::Yes){
			e->Cancel = false;
		}else{
			e->Cancel = true;
			return;
		}

		 try{ 
			//release resources
			 if(capture1){
				 pin_ptr<CvCapture*> p;
				 p = &capture1;
				 cvReleaseCapture( p ); 
			 }
			//cvReleaseCapture(&capture2);
			cvDestroyWindow( "frameWin" );

		 }
		 catch(...){ 
			 //openCVmsgDelegate(" failed to release ressources" );
		 }

		 }

public: static void myMouseHandlerFrame(int mevent, int x, int y, int flags, void* param)
{
  //
	
}



private: System::Void dataGridView1_RowStateChanged(System::Object^  sender, System::Windows::Forms::DataGridViewRowStateChangedEventArgs^  e) {
		 }
private: System::Void dataGridView1_Enter(System::Object^  sender, System::EventArgs^  e) {



		 }



private: System::Void show(int row){
		if (capture1){

			String^ temp = dataGridView1->Rows[row]->Cells["frame"]->Value->ToString();
			int frame = Int32::Parse(temp);

			frameT->Text = "frame: "+frame.ToString();

			int no_of_frames = (int) cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
			if (frame >= no_of_frames){
				frameT->Text = "not found: "+frame.ToString();
				return;
			}
			


			//dont use this => sync problem => key frames
			//cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_FRAMES, frame);
			//frame1 = cvQueryFrame(capture1);


			double frameRate = (double) cvGetCaptureProperty(capture1, CV_CAP_PROP_FPS);
			double frameTime = 1000.0 * (double)frame / (double)frameRate;
			cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, frameTime);

			eyeFrame = cvQueryFrame(capture1);

			int actualFrame = cvGetCaptureProperty(capture1, CV_CAP_PROP_POS_FRAMES)+1;

			if (actualFrame != frame){//we have a problem
				double frameRate = (double) cvGetCaptureProperty(capture1, CV_CAP_PROP_FPS);
				double frameTime = 1000.0 * (double)frame / (double)frameRate - 15000;//set to 15 sec before
				cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, frameTime);
				eyeFrame = cvQueryFrame(capture1);
				actualFrame = cvGetCaptureProperty(capture1, CV_CAP_PROP_POS_FRAMES)+1;

				if(actualFrame < frame){

					//seek from -15seconds before
					for(int i=actualFrame; i < frame; i++){//read until frame
						eyeFrame = cvQueryFrame(capture1);
					}

				}else{//still a problem
				   //do a bruteforce sync
					cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, 0);
					for(int i=0; i <= frame; i++){
						eyeFrame = cvQueryFrame(capture1);
					}

				}
				/*
				actualFrame = cvGetCaptureProperty(capture1, CV_CAP_PROP_POS_FRAMES)+1;
				String^ soll = frame.ToString();
				String^ ist = actualFrame.ToString();
				MessageBox::Show("soll:"+soll+"| ist:" +ist);
				*/
			}


			//visualize old eye point---------------------------------------
			int x,y,tx,ty;
			String^ tempStr;
			//x and y have journal format so they need to be converted (e.g. divided by 2)
			try{tempStr = dataGridView1->Rows[row]->Cells["eye_x"]->Value->ToString();
				x = Int32::Parse(tempStr);}catch(...){x = 0;}
				x = x/2;
			try{tempStr = dataGridView1->Rows[row]->Cells["eye_y"]->Value->ToString();
				y = Int32::Parse(tempStr);}catch(...){y = 0;}
				y = 288 - y/2;

			//template matching coordinates don't need to be converted
			try{tempStr = dataGridView1->Rows[row]->Cells["templateX"]->Value->ToString();
				tx = Int32::Parse(tempStr);}catch(...){tx = -1;}
			try{tempStr = dataGridView1->Rows[row]->Cells["templateY"]->Value->ToString();
				ty = Int32::Parse(tempStr);}catch(...){ty = -1;}

			if ((tx>0) && (ty>0)){
				cvRectangle(eyeFrame,cvPoint(tx-1, ty-1),cvPoint(tx+1, ty+1),CV_RGB(255, 255, 0),2,4,0);
			}
			cvRectangle(eyeFrame,cvPoint(x-1, y-1),cvPoint(x+1, y+1),CV_RGB(0, 255, 0),2,4,0);
			cvShowImage("frameWin", eyeFrame); // display the frame
				
			currentFrame = frame;
		}
	}

private: System::Void dataGridView1_CellEnter(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {


			 if (detecting) return; //if eye detection is in progress return


			 if ((dataGridView1->SelectedRows->Count >0) && (!loading)){
				//int row = dataGridView1->CurrentCell->RowIndex;
				int row = dataGridView1->SelectedRows[0]->Index;
					 if (capture1){

						show(row);
					 }

					 
			}

		 }
private: System::Void nextB_Click(System::Object^  sender, System::EventArgs^  e) {

		int temp = dataGridView1->CurrentCell->RowIndex;
		temp++;

		if (temp < dataGridView1->Rows->Count){
			try{
			dataGridView1->FirstDisplayedScrollingRowIndex = temp;
			dataGridView1->Rows[temp]->Selected = true;
			dataGridView1->CurrentCell = dataGridView1->Rows[temp]->Cells[0];
			show(temp);
			}catch(...){}
		}	

	}


public: Thread ^ mThread; 
public: delegate System::Int32 d_msgCallback(System::Int32, System::Int32); 
public: static d_msgCallback^ msgCallback;

public: System::Int32 updateUI(System::Int32 msg, System::Int32 i){

		   if (this->InvokeRequired) 
		   { 
			   //d_msgCallback^ invoker = gcnew d_msgCallback(this, &Form1::HSmsgCallback);
			   //array<Object^>^ args = { msg };
			   //this->Invoke(invoker, args); 
			   try{
				   this->Invoke(msgCallback,msg,i);

			   }catch(...){return -1;}
		   }else{
			   
			   if (msg == MSG_SELECT_ROW){ dataGridView1->FirstDisplayedScrollingRowIndex = i;}

			   if (msg == MSG_PROGRESS){frameT->Text = "processing frame: "+i.ToString() + "/"+dataGridView1->Rows->Count.ToString();}
			   if (msg == MSG_FORM_OFF){ 
				   panel1->Enabled = false; 
				   for (int i = 0; i < dataGridView1->Columns->Count; i++){
					dataGridView1->Columns[i]->SortMode = DataGridViewColumnSortMode::NotSortable;
				 }
			   }
			   if (msg == MSG_FORM_ON){
				   panel1->Enabled = true; 
				   for (int i = 0; i < dataGridView1->Columns->Count; i++){
					dataGridView1->Columns[i]->SortMode = DataGridViewColumnSortMode::Automatic;
				 }
			   }
			   if (msg == MSG_DONE){done();}
			   
		   }
		   return 0;
		}

public: System::Void done(){

	frameT->Text = "done";

	msgCallback(MSG_FORM_ON, 0);//disable some user interaction with form
}

public: System::Void detectThread(){

	//msgCallback = gcnew d_msgCallback(this, &show::Form1::updateUI); 
	msgCallback(MSG_FORM_OFF, 0);//disable some user interaction with form

	detecting = true; //pupille detection is in progess flag

	if(capture1){				

			int no_of_frames = (int) cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
			cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_FRAMES, 0);

			if (dataGridView1->Rows->Count >0){

				//prepare resultImages used by templateMatching:
				
				cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, 0);//get first frame for width &height
				eyeFrame = cvQueryFrame(capture1);
				IplImage* resultImg[TEMPLATE_COUNT];


				for(int n = 0; n<TEMPLATE_COUNT; n++){
					int resultImg_cols =  eyeFrame->width -  templateFiles[n]->width + 1;
					int resultImg_rows = eyeFrame->height -  templateFiles[n]->height + 1;
					resultImg[n] = cvCreateImage(cvSize(resultImg_cols, resultImg_rows), IPL_DEPTH_32F, 1);
				}

				double frameRate = (double) cvGetCaptureProperty(capture1, CV_CAP_PROP_FPS);

				//find what is smaller no_of_frames in eyeVideo or lines in journalFile 
				int min = no_of_frames;
				if (min > dataGridView1->Rows->Count){min=dataGridView1->Rows->Count;}

				cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, 0);//set again to 0

				for(int i=0;i < min; i++){
					
					msgCallback(MSG_PROGRESS, i);

						//double frameTime = 1000.0 * (double)i / (double)frameRate;
						//cvSetCaptureProperty(capture1, CV_CAP_PROP_POS_MSEC, frameTime);
						eyeFrame = cvQueryFrame(capture1);

						if ((invalidOnlyRadioButton->Checked) && (dataGridView1->Rows[i]->Cells["eye_valid"]->Value->ToString() == "1")){//invalid only is checked but this row is valid
							refreshRowColor(i);
							continue; //  so jump to next round in for
						}


						cv::Mat tempImg(eyeFrame);
						int dilation_size = 3;
						cv::Mat dilation_element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ), cv::Point( dilation_size, dilation_size ) );
						int erosion_size = 7;
						cv::Mat erosion_element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),cv::Point( erosion_size, erosion_size ) );
						dilate(tempImg, tempImg, dilation_element);
						erode(tempImg, tempImg, erosion_element);
						blur(tempImg, tempImg,cv::Size(21,21));

						IplImage* dilEroImg = new IplImage(tempImg);
						//cvShowImage("frameWin", dilEroImg); // display the frame


						int selectedPos = 5; 
						if ((i > selectedPos) && (i <(dataGridView1->Rows->Count - selectedPos))){ msgCallback(MSG_SELECT_ROW, i-selectedPos);}
					

						
						//detect pupil
						
						//convert to gray
						IplImage*  eyeFrameGray = cvCreateImage(cvSize(eyeFrame->width, eyeFrame->height), IPL_DEPTH_8U, 1);
//							cvCvtColor(eyeFrame, eyeFrameGray, CV_RGB2GRAY);
						cvCvtColor(dilEroImg, eyeFrameGray, CV_RGB2GRAY);
						double minVal; 
						double maxVal; 
						CvPoint minLoc; 
						CvPoint maxLoc;
						int templateX = -1;
						int templateY = -1;
						int templateFit = -1;

						for(int n = 0; n<TEMPLATE_COUNT; n++){

							//create resultDataArray (image) used by templateMatching:
							//int resultImg_cols =  eyeFrameGray->width -  templateFiles[n]->width + 1;
							//int resultImg_rows = eyeFrameGray->height -  templateFiles[n]->height + 1;
							//IplImage* resultImg = cvCreateImage(cvSize(resultImg_cols, resultImg_rows), IPL_DEPTH_32F, 1);


							cvMatchTemplate(eyeFrameGray, templateFiles[n], resultImg[n], CV_TM_CCORR_NORMED);
								
 
							cvMinMaxLoc(resultImg[n], &minVal, &maxVal, &minLoc, &maxLoc, 0);
							int maxTemp = Math::Round(maxVal*1000);
							if(maxTemp > templateFit){
								templateFit = maxTemp;
								templateX = maxLoc.x + Math::Round(templateFiles[n]->width /2.0);
								templateY = maxLoc.y + Math::Round(templateFiles[n]->height /2.0);
							}

						}//for TEMP_COUNT

						if(templateX >= 0) dataGridView1->Rows[i]->Cells["templateX"]->Value = templateX;
						if(templateY >= 0) dataGridView1->Rows[i]->Cells["templateY"]->Value = templateY;
						if(templateFit >= 0) dataGridView1->Rows[i]->Cells["templateFit"]->Value = templateFit;

						//release gray images
						if(eyeFrameGray){
							pin_ptr<IplImage*> p;
							p = &eyeFrameGray;
							cvReleaseImage(p);
						}
						/*
						//release temp imag2
						if(tempImg2){
							pin_ptr<IplImage*> p;
							p = &tempImg2;
							cvReleaseImage(p);
						}
						*/

						refreshRowColor(i);

						
				}//for


				//release result images
				for(int n = 0; n<TEMPLATE_COUNT; n++){
					if(resultImg[n]){
						pin_ptr<IplImage*> p;
						p = &resultImg[n];
						cvReleaseImage(p);
					}
				}



			}//if count >0
		}//if capture1


	    detecting = false; //pupile detection is NOT in progess; flag 

		msgCallback(MSG_DONE, 0);//callback done()
}

private: System::Void match_Click(System::Object^  sender, System::EventArgs^  e) {
		  dataGridView1->Sort(dataGridView1->Columns[1], ListSortDirection::Ascending);//sort based on time stamp before start detectionThread
		   mThread = gcnew Thread(gcnew ThreadStart(this,&show::Form1::detectThread));
		   mThread->Start();


		 }

private: System::Void refreshRowColor(int row){

	int data = 10000;

		if ((invalidOnlyRadioButton->Checked) && (dataGridView1->Rows[row]->Cells["eye_valid"]->Value->ToString() == "1")){//invalid only is checked and this row is checked so decolor and return
			dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::White;
			return;
		}

		if(dataGridView1->Rows[row]->Cells["templateFit"]->Value == ""){
			dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::White;
			return;		 
		}

		if (dataGridView1->Rows[row]->Cells["eye_valid"]->Value->ToString() == "0"){
			dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::Gray;
		}

	try{
		String^ temp = dataGridView1->Rows[row]->Cells["templateFit"]->Value->ToString();
		data = Int32::Parse(temp);
		if (data > thresholdUpDown->Value){//above threshold?
			dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::Green;
			dataGridView1->Rows[row]->Cells["aboveThres"]->Value = "Y";
		}else{
			dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::Gray;
			dataGridView1->Rows[row]->Cells["aboveThres"]->Value = "N";
		}


	}catch(...){
		dataGridView1->Rows[row]->DefaultCellStyle->BackColor = Color::White;
		openCVstatus->AppendText("Error while converting in row:" + row.ToString() +"\r\n" );
		//MessageBox::Show( "ERROR while converting..");
	}	
}

private: System::String^ save(System::String^ fn){

	int row,col = -1; 
	try{
			dataGridView1->Sort(dataGridView1->Columns[1], ListSortDirection::Ascending);//sort based on timestamp before saving

			DateTime^ cpCurrentDateTime = DateTime::Now;
			//String^ fn = this->openFileDialog1->FileName;
			String^ dateStr = cpCurrentDateTime->ToString(".yyyy_MM_dd__HH_mm_ss");
			fn = fn + dateStr;
			IO::StreamWriter^ swFileStreamData;

			String^ newLine ="\r\n";
			String^ sep ="\t";


				swFileStreamData = gcnew IO::StreamWriter( fn, true );

				swFileStreamData->Write(firstlineOfTXT );
				swFileStreamData->Write(newLine);
				swFileStreamData->Write(secondlineOfTXT );
				swFileStreamData->Write(newLine);

				for(row=0;row<dataGridView1->Rows->Count; row++){
					for(col=0;col<20; col++){
						String^ temp = dataGridView1->Rows[row]->Cells[col]->Value->ToString();
						swFileStreamData->Write(temp);
						swFileStreamData->Write(sep);
					}
					swFileStreamData->Write(newLine);
				}

				swFileStreamData->Flush();
				swFileStreamData->Close();




	}catch(...){
		//MessageBox::Show( "ERROR while saving.");
		openCVstatus->AppendText("Error while saving. row:"+ row.ToString() + " col:"+ col.ToString()+ "\r\n");
		return "Error while saving. row:"+ row.ToString() + " col:"+ col.ToString();
	}
	return "Saved to: "+fn;

}


private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->openFileDialog1->FileName){
			 String^ r = save(this->openFileDialog1->FileName);
			 MessageBox::Show(r);

		}else{
			MessageBox::Show( "ERROR while saving: No file opened.");
		}

}

private: System::Void acceptB_Click(System::Object^  sender, System::EventArgs^  e) {
			if (dataGridView1->Rows->Count >0){

				dataGridView1->Sort(dataGridView1->Columns[1], ListSortDirection::Ascending);//sort based on timestamp before applying/accept

				int no_of_frames = (int) cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
				int min = no_of_frames;
				if (min > dataGridView1->Rows->Count){min=dataGridView1->Rows->Count;}

				for(int i=0;i<min; i++){
					
					if ((invalidOnlyRadioButton->Checked) && (dataGridView1->Rows[i]->Cells["eye_valid"]->Value->ToString() == "1")){//invalid only is checked but this row is valid
						continue; //  so jump to next round in for
					}

					if((dataGridView1->Rows[i]->Cells["aboveThres"]->Value->ToString() == "Y")){

						int eyeroizoomx,eyeroizoomy,eyeroihoriz,eyeroivert,templateX,templateY;
						double zoomX, zoomY;
						String^ tempStr;
						try{tempStr = dataGridView1->Rows[i]->Cells["eyeroizoomx"]->Value->ToString();
							eyeroizoomx = Int32::Parse(tempStr);}catch(...){continue;}

						try{tempStr = dataGridView1->Rows[i]->Cells["eyeroizoomy"]->Value->ToString();
							eyeroizoomy = Int32::Parse(tempStr);}catch(...){continue;}

						try{tempStr = dataGridView1->Rows[i]->Cells["eyeroihoriz"]->Value->ToString();
							eyeroihoriz = Int32::Parse(tempStr);}catch(...){continue;}

						try{tempStr = dataGridView1->Rows[i]->Cells["eyeroivert"]->Value->ToString();
							eyeroivert = Int32::Parse(tempStr);}catch(...){continue;}

						try{tempStr = dataGridView1->Rows[i]->Cells["templateX"]->Value->ToString();
							templateX = Int32::Parse(tempStr);}catch(...){continue;}

						try{tempStr = dataGridView1->Rows[i]->Cells["templateY"]->Value->ToString();
							templateY = Int32::Parse(tempStr);}catch(...){continue;}

						zoomX = 384.0 / (double)eyeroizoomx;
						zoomY = -288.0 / (double)eyeroizoomy;


						//transform to journal format => x *2  &  y*2
						//openCv = 288 - eye_y/2;
						//(openCv -288) *-2 = eye_y
						int convertedX = (templateX ) *2; 
						int convertedY = (templateY - 288) *-2; 

						dataGridView1->Rows[i]->Cells["eye_x"]->Value = convertedX.ToString();
						dataGridView1->Rows[i]->Cells["eye_y"]->Value = convertedY.ToString();

						double field_x = 384.0 + ((double)convertedX * zoomX) - (2.0 * zoomX * (double)eyeroihoriz);
						double field_y = 288.0 + ((double)convertedY * zoomY) - (2.0 * zoomY * (double)eyeroivert);


						dataGridView1->Rows[i]->Cells["field_x"]->Value = Math::Round(field_x).ToString();
						dataGridView1->Rows[i]->Cells["field_y"]->Value = Math::Round(field_y).ToString();

						dataGridView1->Rows[i]->Cells["templateX"]->Value = "";
						dataGridView1->Rows[i]->Cells["templateY"]->Value = "";
						dataGridView1->Rows[i]->Cells["templateFit"]->Value = "";
						dataGridView1->Rows[i]->Cells["aboveThres"]->Value = "";
						dataGridView1->Rows[i]->Cells["eye_valid"]->Value = "1";//set valid


						refreshRowColor(i);
					}else{
						if(markInvalidRadioButton->Checked){
							dataGridView1->Rows[i]->Cells["templateX"]->Value = "";
							dataGridView1->Rows[i]->Cells["templateY"]->Value = "";
							dataGridView1->Rows[i]->Cells["templateFit"]->Value = "";
							dataGridView1->Rows[i]->Cells["aboveThres"]->Value = "";
							dataGridView1->Rows[i]->Cells["eye_valid"]->Value = "0";//set invalid
						}
					}

				}

			}//if count >0
		 }
private: System::Void button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
			MessageBox::Show("This is NOT an official Dikablis(tm)/Ergoneers(tm) tool! Use at your own risk.\r\n------------------------\r\n"+
"3-clause BSD License\r\nCopyright (c) 2014-2016 Michael Krause (krause@tum.de) Institute of Ergonomics, Technische Universität München."+
"All rights reserved.\r\n"+
"Redistribution and use in source and binary forms, with or without\r\n"+
"modification, are permitted provided that the following conditions are met:\r\n"+
"    * Redistributions of source code must retain the above copyright\r\n"+
"      notice, this list of conditions and the following disclaimer.\r\n"+
"    * Redistributions in binary form must reproduce the above copyright\r\n"+
"      notice, this list of conditions and the following disclaimer in the\r\n"+
"      documentation and/or other materials provided with the distribution.\r\n"+
"    * Neither the name of the <organization> nor the\r\n"+
"      names of its contributors may be used to endorse or promote products\r\n"+
"      derived from this software without specific prior written permission.\r\n\r\n"+

"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS' AND"+
"ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"+
"WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"+
"DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY"+
"DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"+
"(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"+
"LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"+
"ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"+
"(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"+
"SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."+
"\r\n-------------------\r\nThis software thanks and uses openCv (BSD licence) and Qt (e.g., LGPL) via calls. If this is the compiled executable there is a readme.txt with the licence texts.");
		 }
private: System::Void thresholdUpDown_ValueChanged_1(System::Object^  sender, System::EventArgs^  e) {
			refresh();
		 }

private: System::Void refresh() {
			if (dataGridView1->Rows->Count >0){

				//dataGridView1->Sort(dataGridView1->Columns[1], ListSortDirection::Ascending);

				int no_of_frames = (int) cvGetCaptureProperty(capture1, CV_CAP_PROP_FRAME_COUNT);
				int min = no_of_frames;
				if (min > dataGridView1->Rows->Count){min=dataGridView1->Rows->Count;}

				for(int i=0;i<min; i++){

					refreshRowColor(i);
				}

			}//if count >0

		 }
};
}

