/*==================================

	[input.cpp]
	Author : �o���đ�

====================================*/

#include "input.h"
#include "main.h"

//	�v���g�^�C�v�錾
static bool Init(HINSTANCE hInstance);
static void Uninit();

//	�O���[�o���ϐ�
LPDIRECTINPUT8			g_pInput = NULL;	//	DirectInput�I�u�W�F�N�g

//	�L�[�{�[�h
LPDIRECTINPUTDEVICE8	Input::g_pDevKeyboard = NULL;			//	�L�[�{�[�h�f�o�C�X
BYTE					Input::g_aKeyState[NUM_KEY_MAX];		//	�L�[�{�[�h���i�����Ă���ԁj
BYTE					Input::g_aKeyStateTrigger[NUM_KEY_MAX];	//	�L�[�{�[�h���i�������u�ԁj
BYTE					Input::g_aKeyStateRelease[NUM_KEY_MAX];	//	�L�[�{�[�h���i�������u�ԁj
	
//	�Q�[���p�b�h
LPDIRECTINPUTDEVICE8	Input::g_pGamePad[GAMEPADMAX] = { NULL, NULL, NULL, NULL };	// �p�b�h�f�o�C�X
DWORD					Input::g_padState[GAMEPADMAX];								// �p�b�h���i�����Ă���ԁj
DWORD					Input::g_padTrigger[GAMEPADMAX];							// �p�b�h���i�������u�ԁj
int						Input::g_padCount = 0;										// ���o�����p�b�h�̐�
			
//	DirectInput�I�u�W�F�N�g������
bool Init(HINSTANCE hInstance)
{
	//	DirectInput8�̍쐬
	if (g_pInput == NULL) 
	{
		//	�쐬�̎��s
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{

			return false;
		}
	}

	return true;	//����I��
}

//�L�[�{�[�h�̏���������
bool Input::KB_Init(HINSTANCE hInstance, HWND hWnd)
{
	if (!Init(hInstance)) 
	{

		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}
	//													 DISCL_BACKGROUND | DISCL_EXCLUSIVE	
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return false;
	}

	g_pDevKeyboard->Acquire();

	return true;
}

//Input�̏I������
void Input::Uninit()
{
	DEVICE_RELEASE(g_pDevKeyboard);
	for (int i = 0; i < GAMEPADMAX; i++) {
		if (g_pGamePad[i])
		{
			g_pGamePad[i]->Unacquire();
			g_pGamePad[i]->Release();
		}
	}
	DEVICE_RELEASE(g_pInput);
}

//�L�[�{�[�h�̍X�V����
void Input::KB_Update()
{
	BYTE aKeyState[NUM_KEY_MAX];

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//�L�[�{�[�h�������Ă����
bool Input::KB_IsPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h���������u��
bool Input::KB_IsTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�𗣂����Ƃ�
bool Input::KB_IsRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//�Q�[���p�b�h�p�R�[���o�b�N�֐�
BOOL CALLBACK Input::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pInput->CreateDevice(lpddi->guidInstance, &g_pGamePad[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

//�Q�[���p�b�h�̏���������
bool Input::GP_Init(HINSTANCE hInstance, HWND hWnd)
{
	if (!Init(hInstance)) 
	{

		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return false;
	}

	HRESULT		result;
	int			i;

	g_padCount = 0;
	// �W���C�p�b�h��T��
	g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i<g_padCount; i++) 
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = g_pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//		if ( FAILED(result) )
		//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		g_pGamePad[i]->Acquire();
	}

	return true;
}

//�Q�[���p�b�h�̍X�V����
void Input::GP_Update()
{
	HRESULT			result;	
	DIJOYSTATE2		dijs;	//�Q�[���p�b�h�̓��͒l
	int				i;

	for (i = 0; i<g_padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = g_padState[i];	//�Q�[���p�b�h�̃{�^���z��
		g_padState[i] = 0x00000000l;	// ���͗p32�r�b�g�̏�����

		result = g_pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) 
		{
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		result = g_pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) 
		{
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}
		
		/*---------------------------------------------------------------------------------------------------
			�R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���//long�^�f�t�H���g�i32767���炢�j   
		-----------------------------------------------------------------------------------------------------*/
		
		//�@��	����
		//! �ԁ@Xbox�̂�
		//" �@PS4�̂�
		//!	���Ӂ@Xbox��PS4�̓��͂𓯎��Ɏg��Ȃ�����

		// Xbox��PS4:���X�e�B�b�Ny-axis (forward)
		if (dijs.lY < 0)					g_padState[i] |= LEFTSTICK_UP;
		// Xbox��PS4:���X�e�B�b�Ny-axis (backward)
		if (dijs.lY > 0)					g_padState[i] |= LEFTSTICK_DOWN;
		// Xbox��PS4:���X�e�B�b�Nx-axis (left)
		if (dijs.lX < 0)					g_padState[i] |= LEFTSTICK_LEFT;
		// Xbox��PS4:���X�e�B�b�Nx-axis (right)
		if (dijs.lX > 0)					g_padState[i] |= LEFTSTICK_RIGHT;
		
		//! Xbox:�E�X�e�B�b�Ny-axis (forward)
		if (dijs.lRy < 16383)				g_padState[i] |= XboxRIGHTSTICK_UP;
		//! Xbox:�E�X�e�B�b�Ny-axis (backward)
		if (dijs.lRy > 49150)				g_padState[i] |= XboxRIGHTSTICK_DOWN;
		//! Xbox:�E�X�e�B�b�Nx-axis (left)
		if (dijs.lRx < 16383)				g_padState[i] |= XboxRIGHTSTICK_LEFT;
		//! Xbox:�E�X�e�B�b�Nx-axis (right)
		if (dijs.lRx > 49150)				g_padState[i] |= XboxRIGHTSTICK_RIGHT;
	
		//" PS4:�E�X�e�B�b�Ny-axis (forward)
		if (dijs.lRz < 16383)				g_padState[i] |= PS4RIGHTSTICK_UP;
		//" PS4:�E�X�e�B�b�Ny-axis (backward)
		if (dijs.lRz > 49150)				g_padState[i] |= PS4RIGHTSTICK_DOWN;
		//" PS4:�E�X�e�B�b�Nx-axis (left)
		if (dijs.lZ < 16383)				g_padState[i] |= PS4RIGHTSTICK_LEFT;
		//" PS4:�E�X�e�B�b�Nx-axis (right)
		if (dijs.lZ > 49150)				g_padState[i] |= PS4RIGHTSTICK_RIGHT;

		//! Xbox:LT�g���K�[
		if (dijs.lZ > 49150)				g_padState[i] |= TRIGGER_LT;
		//! Xbox:RT�g���K�[
		if (dijs.lZ < 16383)				g_padState[i] |= TRIGGER_RT;
		//" PS4:L2�g���K�[
		if (dijs.lRx > 10000)				g_padState[i] |= TRIGGER_L2;
		//" PS4:R2�g���K�[
		if (dijs.lRy > 10000)				g_padState[i] |= TRIGGER_R2;
		
		//* Xbox:�`�{�^��	�@PS4�F���{�^��
		if (dijs.rgbButtons[0] & 0x80)		g_padState[i] |= BUTTON_1;
		//* Xbox:�w�{�^��	  PS4�F�Z�{�^��
		if (dijs.rgbButtons[2] & 0x80)		g_padState[i] |= BUTTON_2;
		//* Xbox:�a�{�^��	�@PS4�F�~�{�^��
		if (dijs.rgbButtons[1] & 0x80)		g_padState[i] |= BUTTON_3;
		//*	Xbox:�x�{�^��	�@PS4�F���{�^��
		if (dijs.rgbButtons[3] & 0x80)		g_padState[i] |= BUTTON_4;
		//* Xbox:LB�{�^��	�@PS4�FL1�{�^��
		if (dijs.rgbButtons[4] & 0x80)		g_padState[i] |= BUTTON_LB;
		//* Xbox:RB�{�^��	�@PS4�FR1�{�^��
		if (dijs.rgbButtons[5] & 0x80)		g_padState[i] |= BUTTON_RB;
		//* Xbox:BACK�{�^�� �@PS4 : OPTION�{�^��
		if (dijs.rgbButtons[6] & 0x80)		g_padState[i] |= BUTTON_BACK;
		//* Xbox:START�{�^���@PS4 : SHARE�{�^��
		if (dijs.rgbButtons[7] & 0x80)		g_padState[i] |= BUTTON_START;
		
		//! Xbox:L3�{�^��	
		if (dijs.rgbButtons[8] & 0x80)		g_padState[i] |= BUTTON_L3;
		//! Xbox:R3�{�^��	
		if (dijs.rgbButtons[9] & 0x80)		g_padState[i] |= BUTTON_R3;
		
		//* Xbox��PS4�F�����L�[��
		if (dijs.rgdwPOV[0] <= 0 )									g_padState[i] |= BUTTON_UP;
		//* Xbox��PS4�F�����L�[��
		if (dijs.rgdwPOV[0] <= 18000 && dijs.rgdwPOV[0] > 9000)		g_padState[i] |= BUTTON_DOWN;
		//* Xbox��PS4�F�����L�[��
		if (dijs.rgdwPOV[0] <= 27000 && dijs.rgdwPOV[0] > 18000)	g_padState[i] |= BUTTON_LEFT;
		//* Xbox��PS4�F�����L�[�E
		if (dijs.rgdwPOV[0] <= 9000 && dijs.rgdwPOV[0] > 0)			g_padState[i] |= BUTTON_RIGHT;
			   
		// Trigger�ݒ�
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// �O��ƈ���Ă���
			& g_padState[i]);					// ��������ON�̂��

	}
}

//�Q�[���p�b�h�̃{�^���������Ă����
BOOL Input::GP_IsPress(int padNo, DWORD button)
{
	return (button & g_padState[padNo]);
}

//�Q�[���p�b�h�̃{�^�����������u��
BOOL Input::GP_IsTrigger(int padNo, DWORD button)
{
	return (button & g_padTrigger[padNo]);
}

//bool Input::Mouse_Init(HINSTANCE hInstance, HWND hWnd)
//{
//	if (!Init(hInstance))
//	{
//
//		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
//		return false;
//	}
//
//	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
//	{
//		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
//		return false;
//	}
//
//	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
//	{
//		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
//		return false;
//	}
//	//													 DISCL_BACKGROUND | DISCL_EXCLUSIVE	
//	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
//	{
//		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
//		return false;
//	}
//
//	g_pDevKeyboard->Acquire();
//
//	return true;
//}
//
//void Input::Mouse_Update()
//{
//}
//
//bool Input::Mouse_IsTrigger(int nMouse)
//{
//	return false;
//}
//
//
