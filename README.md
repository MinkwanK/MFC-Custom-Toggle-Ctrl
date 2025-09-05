# Custom Toggle - MFC  
![MFC_Custom_Toggle](https://github.com/user-attachments/assets/1c970728-eb10-479a-9c99-fb39631b0d93)

**You can simply use Toggle UI by using custom toggle that I made.**  
<br>
<img width="242" height="234" alt="image" src="https://github.com/user-attachments/assets/74d78d28-b660-4e08-a2a7-b4d55557e6a7" />

## Features
- **You can hide or show content as you wish.**  
- **Supports scrolling.**  
- **Supports a toggle menu (you can enable or disable it by clicking the toggle icon at the top).**

## How to Use
1. **Include the following classes in your project: `TogglePanel`, `ToggleBoard`, `ToggleItem`**  
2. **You need to init Panel object.**
```cpp
CTogglePanel m_panel;
```
```cpp
m_panel.Init(this, TOGGLE_ICON_TYPE::USE_24);
```
3. **Add Child Dialog to panel**
```cpp
m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample, 
    IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
    IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);
```

## Example
```cpp
void CMKTestDlg::Init()
{
    CRect rc;
    GetClientRect(&rc);
    rc.left = 100;
    rc.top = 0;
    rc.right = 300;
    rc.bottom = rc.bottom - 5;

    m_panel.Init(this, TOGGLE_ICON_TYPE::USE_24);
    CString sValue = _T("TEST 1");
    m_sample.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample.ShowWindow(SW_HIDE);
    CFont* pFont = this->GetFont();
    m_panel.AddToggleTitle(sValue, 60, 60,  pFont, &m_sample, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    sValue.Format(_T("TEST 2"));
    m_sample2.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample2.ShowWindow(SW_HIDE);
    m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample2, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    sValue.Format(_T("TEST 3"));
    m_sample3.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample3.ShowWindow(SW_HIDE);
    m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample3, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    m_bInit = TRUE;
}
```

## ETC
- **You need to prepare toggle icon.**  
- **Ask me if you have any problem.**

---
**한국어 버전**<br><br>
**제가 만든 Custom Toggle을 사용하면 MFC 내부에서 간단하게 Toggle UI를 구현할 수 있습니다.**  
<br>
<img width="242" height="234" alt="image" src="https://github.com/user-attachments/assets/74d78d28-b660-4e08-a2a7-b4d55557e6a7" />

## 기능
- **원하는 대로 콘텐츠를 숨기거나 표시할 수 있습니다.**  
- **스크롤 기능을 지원합니다.**  
- **상단 토글 아이콘을 클릭하여 토글 메뉴를 활성화/비활성화할 수 있습니다.**

## 사용법
1. **프로젝트에 다음 클래스를 포함해야 합니다: `TogglePanel`, `ToggleBoard`, `ToggleItem`**  
2. **Panel 객체를 초기화해야 합니다.**
```cpp
CTogglePanel m_panel;
```
```cpp
m_panel.Init(this, TOGGLE_ICON_TYPE::USE_24);
```
3. **자식 다이얼로그를 Panel에 추가합니다**
```cpp
m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample, 
    IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
    IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);
```

## 예제
```cpp
void CMKTestDlg::Init()
{
    CRect rc;
    GetClientRect(&rc);
    rc.left = 100;
    rc.top = 0;
    rc.right = 300;
    rc.bottom = rc.bottom - 5;

    m_panel.Init(this, TOGGLE_ICON_TYPE::USE_24);
    CString sValue = _T("TEST 1");
    m_sample.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample.ShowWindow(SW_HIDE);
    CFont* pFont = this->GetFont();
    m_panel.AddToggleTitle(sValue, 60, 60,  pFont, &m_sample, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    sValue.Format(_T("TEST 2"));
    m_sample2.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample2.ShowWindow(SW_HIDE);
    m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample2, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    sValue.Format(_T("TEST 3"));
    m_sample3.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
    m_sample3.ShowWindow(SW_HIDE);
    m_panel.AddToggleTitle(sValue, 60, 60, pFont, &m_sample3, 
        IDI_ICON_UP, IDI_ICON_UP_DISABLE, IDI_ICON_UP, 
        IDI_ICON_UP_CLICK, IDI_ICON_DOWN, IDI_ICON_DOWN_CLICK);

    m_bInit = TRUE;
}
```

## 기타
- **토글 아이콘 이미지를 따로 준비하셔서 리소스 파일로 등록하셔야 합니다.**  
- **문제가 생기면 편하게 문의주세요.**

