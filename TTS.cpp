#include "stdafx.h"
#include <sapi.h>
#include <sphelper.h>
#include <atlbase.h>
#include <vector>
#include "TTS.h"

CComPtr<ISpVoice> pVoice = nullptr;
std::vector<CComPtr<ISpObjectToken>> voicetokens;
bool InitTTS()
{
	if (SUCCEEDED(pVoice.CoCreateInstance(CLSID_SpVoice)))
	{
		CComPtr<IEnumSpObjectTokens> cpEnum;
		if (SUCCEEDED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum)))
		{
			ULONG ulCount;
			HRESULT hr = cpEnum->GetCount(&ulCount);

			// Obtain a list of available voice tokens, set
			// the voice to the token, and call Speak.
			while (SUCCEEDED(hr) && ulCount--)
			{
				CComPtr<ISpObjectToken> cpVoiceToken;
				hr = cpEnum->Next(1, &cpVoiceToken, NULL);
				if (SUCCEEDED(hr))
					voicetokens.push_back(cpVoiceToken);
			}
		}
		return true;
	}
	return false;
}

void DoTTS(const char* text)
{
	int sz = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
	wchar_t* wtext = new wchar_t[sz];
	MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, sz);
	if (voicetokens.size() > 0)
		pVoice->SetVoice(voicetokens[rand() % voicetokens.size()]);
	pVoice->Speak(wtext, SPF_ASYNC | SPF_PURGEBEFORESPEAK | SPF_IS_NOT_XML, nullptr);
	delete[] wtext;
}
