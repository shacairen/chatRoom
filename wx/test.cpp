
// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".

#include <wx/wx.h>
#include <wx/listctrl.h>
#include<wx/mstream.h>
#include <wx/richtext/richtextctrl.h>
#include"Client.h"

static Client c;

class ChatRoomFrame : public wxFrame {
private:
	wxTextCtrl* chatText;
	wxTextCtrl* inputText;
	wxButton* sendButton;
	wxButton* sendPicture;

	void OnQuit(wxCommandEvent& event) {
		Close();
	}

	void OnSend(wxCommandEvent& event) {
		wxString text = inputText->GetValue().Strip(wxString::both);
		if (!text.empty()) {
			Msg m;
			string mess =inputText->GetValue().ToStdString();
			m.WriteName(MyName);
			m.WriteMess(mess);
			m.WriteWork(3);
			m.WriteFlag(0);
			send(c.GetSocket(), (char*)&m, sizeof(Msg), 0);
			inputText->SetValue("");
		}
		inputText->SetFocus();
	}

	void OnSendPicture(wxCommandEvent& event) {

		wxDirDialog dialog(this, "Select a folder", wxEmptyString, wxDD_DEFAULT_STYLE);

		if (dialog.ShowModal() == wxID_OK)
		{
			wxString selectedPath = dialog.GetPath();
			string ImagePath = selectedPath.ToStdString();
			std::ifstream inputFile(ImagePath, std::ios::binary | std::ios::ate);
			wxMessageDialog* messageDialog = nullptr;
			if (!inputFile.is_open()) {
				// 处理文件打开失败的情况
				messageDialog = new wxMessageDialog(this, "File open failed!", "Error", wxOK | wxICON_ERROR);
				return;
			}

			std::streamsize fileSize = inputFile.tellg();
			inputFile.seekg(0, std::ios::beg);

			std::string imageData(fileSize, '\0');
			if (!inputFile.read(&imageData[0], fileSize)) {
				// 处理读取文件失败的情况
				messageDialog = new wxMessageDialog(this, "File read failed!", "Error", wxOK | wxICON_ERROR);
				return;
			}

			Msg m;
			m.WriteName(MyName);
			m.WriteMess(imageData);
			m.WriteWork(4);
			m.WriteFlag(0);
			send(c.GetSocket(), (char*)&m, sizeof(Msg), 0);

		}

	}

public:
	ChatRoomFrame(const wxString& title);
	void AppendMessage(const wxString& msg) {
		chatText->AppendText(msg + "\n");
	}

	void AppendPicture(string& imageData) {

		wxMemoryInputStream stream(imageData.data(), imageData.size());
		wxImage image(stream, wxBITMAP_TYPE_ANY);

		wxBitmap bitmap(image);

		chatText->Freeze();//冻结

		

		chatText->Thaw();//解冻
		chatText->Refresh();

	}

};

static void RecvClient(ChatRoomFrame *chat) {
	while (true) {
		Msg m;
		if (recv(c.GetSocket(), (char *)&m, sizeof(m), 0) <= 0)
		{
			wxMessageDialog* messageDialog = new wxMessageDialog(NULL, "Please fill in all fields!", "Error", wxOK | wxICON_ERROR);
			messageDialog->ShowModal();
			break;
		}
		switch (m.GetWork())
		{
		case 3: {//群聊
			wxString name(m.GetName()+":");
			chat->AppendMessage(name);
			wxString mess(m.GetMess());
			chat->AppendMessage(mess);
			break;
		}
		case 4: {//发送图片
			string imageData = m.GetMess();
			chat->AppendPicture(imageData);
			break;
		}
		case 6://在线名单
			break;
		}
	}
}

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnRegister(wxCommandEvent& event);
	void OnLogin(wxCommandEvent& event);
private:
	wxPanel* m_mainPanel;
	wxButton* m_registerButton;
	wxButton* m_loginButton;
};



ChatRoomFrame::ChatRoomFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200)) {
	// 创建聊天室界面的控件
	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, "&Quit\tAlt-F4", "Quit the application");
	menuBar->Append(fileMenu, "&File");
	SetMenuBar(menuBar);

	// 创建聊天框和输入框
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);//垂直的
	chatText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	inputText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);//水平的
	hbox->Add(inputText, wxSizerFlags(1).Expand().Border(wxALL, 5));
	sendButton = new wxButton(this, wxID_ANY, "Send");
	sendPicture = new wxButton(this, wxID_ANY, "+");
	hbox->Add(sendButton, wxSizerFlags(0).Border(wxALL, 5));
	hbox->Add(sendPicture, wxSizerFlags(0).Border(wxALL, 5));
	vbox->Add(chatText, wxSizerFlags(1).Expand().Border(wxALL, 5));
	vbox->Add(hbox, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, 5));

	/*
	wxBoxSizer *thox = new wxBoxSizer(wxHORIZONTAL);
	m_onlineUsersListCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	m_onlineUsersListCtrl->InsertColumn(0, "Online Users", wxLIST_FORMAT_LEFT, 150); // 添加列表列
	thox->Add(m_onlineUsersListCtrl,wxSizerFlags(1).Expand().Border(wxALL,5));
	thox->Add(vbox, wxSizerFlags(1).Expand().Border(wxLEFT, 5));
	//测试*/

	// 设置窗口布局
	SetSizer(vbox);
	Layout();

	// 绑定事件处理函数
	Bind(wxEVT_MENU, &ChatRoomFrame::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_BUTTON, &ChatRoomFrame::OnSend, this, sendButton->GetId());
	Bind(wxEVT_BUTTON, &ChatRoomFrame::OnSendPicture, this, sendPicture->GetId());
	Bind(wxEVT_TEXT_ENTER, &ChatRoomFrame::OnSend, this, inputText->GetId());

	// 初始化聊天框和输入框的光标
	inputText->SetFocus();
	chatText->SetFocus();

	tp.PushFront(RecvClient,this);
}

class RegisterFrame : public wxFrame
{
public:
	RegisterFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnRegister(wxCommandEvent& event);
private:
	wxPanel* m_mainPanel;
	wxStaticText* m_accountLabel;
	wxStaticText* m_usernameLabel;
	wxStaticText* m_passwordLabel;
	wxTextCtrl* m_accountText;
	wxTextCtrl* m_usernameText;
	wxTextCtrl* m_passwordText;
	wxButton* m_registerButton;
};

class LoginFrame : public wxFrame
{
public:
	LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnLogin(wxCommandEvent& event);
private:
	wxPanel* m_mainPanel;
	wxStaticText* m_accountLabel;
	wxStaticText* m_passwordLabel;
	wxTextCtrl* m_accountText;
	wxTextCtrl* m_passwordText;
	wxButton* m_loginButton;
};

enum
{
	ID_REGISTER = wxID_HIGHEST + 1,
	ID_LOGIN
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame("My App", wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_mainPanel = new wxPanel(this, wxID_ANY);
	m_registerButton = new wxButton(m_mainPanel, ID_REGISTER, "Register", wxPoint(50, 50), wxSize(150, 50));
	m_loginButton = new wxButton(m_mainPanel, ID_LOGIN, "Login", wxPoint(250, 50), wxSize(150, 50));
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnRegister, this, ID_REGISTER);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnLogin, this, ID_LOGIN);
}

void MyFrame::OnRegister(wxCommandEvent& event)
{
	RegisterFrame* registerFrame = new RegisterFrame("Register", wxPoint(50, 50), wxSize(450, 340));
	registerFrame->Show(true);
}

void MyFrame::OnLogin(wxCommandEvent& event)
{
	LoginFrame* loginFrame = new LoginFrame("Login", wxPoint(50, 50), wxSize(450, 340));
	loginFrame->Show(true);
}

RegisterFrame::RegisterFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_mainPanel = new wxPanel(this, wxID_ANY);
	m_accountLabel = new wxStaticText(m_mainPanel, wxID_ANY, "Account:", wxPoint(50, 50), wxSize(100, 30));
	m_passwordLabel = new wxStaticText(m_mainPanel, wxID_ANY, "Password:", wxPoint(50, 100), wxSize(100, 30));
	m_usernameLabel = new wxStaticText(m_mainPanel, wxID_ANY, "UserName:", wxPoint(50, 150), wxSize(200, 30));
	m_accountText = new wxTextCtrl(m_mainPanel, wxID_ANY, wxEmptyString, wxPoint(160, 50), wxSize(200, 30));
	m_passwordText = new wxTextCtrl(m_mainPanel, wxID_ANY, wxEmptyString, wxPoint(160, 100), wxSize(200, 30), wxTE_PASSWORD);
	m_usernameText = new wxTextCtrl(m_mainPanel, wxID_ANY, wxEmptyString, wxPoint(160, 150), wxSize(200, 30));
	m_registerButton = new wxButton(m_mainPanel, wxID_OK, "Register", wxPoint(160, 200), wxSize(100, 50));
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &RegisterFrame::OnRegister, this, wxID_OK);
}

void RegisterFrame::OnRegister(wxCommandEvent& event)
{
	wxString account = m_accountText->GetValue();
	wxString username = m_usernameText->GetValue();
	wxString password = m_passwordText->GetValue();
	// 实现注册逻辑，成功或失败后弹出提示框
	wxMessageDialog* messageDialog = nullptr;
	if (account.IsEmpty() || username.IsEmpty() || password.IsEmpty())
	{
		messageDialog = new wxMessageDialog(this, "Please fill in all fields!", "Error", wxOK | wxICON_ERROR);
	}
	else
	{
		Msg m;
		m.WriteAccount(account.ToStdString());
		m.WriteName(username.ToStdString());
		m.WritePassword(password.ToStdString());
		int res = NewUser(c.GetSocket(), m);
		switch (res)
		{
		case 1:
			messageDialog = new wxMessageDialog(this, "Register successfully!pls Login!", "Success", wxOK | wxICON_INFORMATION);
			break;
		case -1:
			messageDialog = new wxMessageDialog(this, "Server fail!", "Fail", wxOK | wxICON_INFORMATION);
			break;
		case 2:
			messageDialog = new wxMessageDialog(this, "Register fail!Account exists", "Fail", wxOK | wxICON_INFORMATION);
			break;
		case 3:
			messageDialog = new wxMessageDialog(this, "Register fail!UserName exists", "Fail", wxOK | wxICON_INFORMATION);
			break;
		}
		// 实现注册逻辑
	}
	messageDialog->ShowModal();
	Destroy();
}

LoginFrame::LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_mainPanel = new wxPanel(this, wxID_ANY);
	m_accountLabel = new wxStaticText(m_mainPanel, wxID_ANY, "Account:", wxPoint(50, 50), wxSize(100, 30));
	m_passwordLabel = new wxStaticText(m_mainPanel, wxID_ANY, "Password:", wxPoint(50, 100), wxSize(100, 30));
	m_accountText = new wxTextCtrl(m_mainPanel, wxID_ANY, wxEmptyString, wxPoint(160, 50), wxSize(200, 30));
	m_passwordText = new wxTextCtrl(m_mainPanel, wxID_ANY, wxEmptyString, wxPoint(160, 100), wxSize(200, 30), wxTE_PASSWORD);
	m_loginButton = new wxButton(m_mainPanel, wxID_OK, "Login", wxPoint(160, 150), wxSize(100, 50));
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LoginFrame::OnLogin, this, wxID_OK);
}

void LoginFrame::OnLogin(wxCommandEvent& event)
{
	wxString account = m_accountText->GetValue();
	wxString password = m_passwordText->GetValue();
	// 实现登录逻辑，成功或失败后弹出提示框
	wxMessageDialog* messageDialog = nullptr;
	if (account.IsEmpty() || password.IsEmpty())
	{
		messageDialog = new wxMessageDialog(this, "请填写完成!!", "Error", wxOK | wxICON_ERROR);
	}
	else
	{
		Msg m;
		m.WriteAccount(account.ToStdString());
		m.WritePassword(password.ToStdString());
		int res=Enter(c.GetSocket(), m);
		// 实现登录逻辑
		switch (res)
		{
		case 1: {
			messageDialog = new wxMessageDialog(this, "Login successfully!", "Success", wxOK | wxICON_INFORMATION);
			ChatRoomFrame* chatRoom = new ChatRoomFrame("Chat Room");
			chatRoom->Show(true);
			break;
		}
		case -1:
			messageDialog = new wxMessageDialog(this, "Server fail!", "Fail", wxOK | wxICON_INFORMATION);
			break;
		case 3:
			messageDialog = new wxMessageDialog(this, "Login fail!", "Fail", wxOK | wxICON_INFORMATION);
			break;
		}
	}
	messageDialog->ShowModal();
	Destroy();
}

