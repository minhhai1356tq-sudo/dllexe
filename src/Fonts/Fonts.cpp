#include "Fonts.hpp"
#include "FontAwesome.hpp"
#include "FontsTwo.hpp"
#include "FontInter.hpp"
#include "Imagens/LogoBytes.hpp"
#include <wincodec.h>
#include <vector>
#pragma comment(lib, "windowscodecs.lib")

namespace FWork {
    void Fonts::Initialize(ID3D11Device* Device) {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig FontAwesomeConfig;
        FontAwesomeConfig.GlyphMinAdvanceX = 25.f * (2.0f / 3.0f);

        static const ImWchar IconRanges[] = {
            ICON_MIN_FA, ICON_MAX_FA, 0
        };

        InterBlack = io.Fonts->AddFontFromMemoryCompressedTTF(InterBlack_compressed_data, InterBlack_compressed_size, 14);
        InterBold = io.Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data, InterBold_compressed_size, 17);
        InterBold12 = io.Fonts->AddFontFromMemoryCompressedTTF(InterBold_compressed_data, InterBold_compressed_size, 15);
        InterExtraBold = io.Fonts->AddFontFromMemoryCompressedTTF(InterExtraBold_compressed_data, InterExtraBold_compressed_size, 13);
        InterExtraLight = io.Fonts->AddFontFromMemoryCompressedTTF(InterExtraLight_compressed_data, InterExtraLight_compressed_size, 14);
        InterLight = io.Fonts->AddFontFromMemoryCompressedTTF(InterLight_compressed_data, InterLight_compressed_size, 12);
        InterMedium = io.Fonts->AddFontFromMemoryCompressedTTF(InterMedium_compressed_data, InterMedium_compressed_size, 17);
        InterRegular = io.Fonts->AddFontFromMemoryCompressedTTF(InterRegular_compressed_data, InterRegular_compressed_size, 17);
        InterRegular14 = io.Fonts->AddFontFromMemoryCompressedTTF(InterRegular_compressed_data, InterRegular_compressed_size, 15);
        InterSemiBold = io.Fonts->AddFontFromMemoryCompressedTTF(InterSemiBold_compressed_data, InterSemiBold_compressed_size, 16);
        InterThin = io.Fonts->AddFontFromMemoryCompressedTTF(InterThin_compressed_data, InterThin_compressed_size, 14);

        GeistRegular = io.Fonts->AddFontFromMemoryCompressedTTF(GeistRegular_compressed_data, GeistRegular_compressed_size, 16);
        GeistRegularMedium = io.Fonts->AddFontFromMemoryCompressedTTF(GeistRegular_compressed_data, GeistRegular_compressed_size, 18);
        GeistMedium = io.Fonts->AddFontFromMemoryCompressedTTF(GeistRegular_compressed_data, GeistRegular_compressed_size, 14);
        GeistBold = io.Fonts->AddFontFromMemoryCompressedTTF(GeistBold_compressed_data, GeistBold_compressed_size, 36);
        GeistBoldMedium = io.Fonts->AddFontFromMemoryCompressedTTF(GeistBold_compressed_data, GeistBold_compressed_size, 16);

        ImFontConfig customFontConfig;
        customFontConfig.MergeMode = true;
        customFontConfig.OversampleH = 1;
        customFontConfig.OversampleV = 1;
        customFontConfig.PixelSnapH = true;

        static const ImWchar customRanges[] = { 0xe000, 0xe204, 0x00 };
        IconWeapon = io.Fonts->AddFontFromMemoryCompressedTTF(weapon_compressed_data, weapon_compressed_size, 41.0f, &customFontConfig, customRanges);


        FontAwesomeRegular = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeRegular_compressed_data, FontAwesomeRegular_compressed_size, 25.f * (2.0f / 3.0f), &FontAwesomeConfig, &IconRanges[0]);
        FontAwesomeSolid = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeSolid_compressed_data, FontAwesomeSolid_compressed_size, 27.f * (2.0f / 3.0f), &FontAwesomeConfig, &IconRanges[0]);
        FontAwesomeSolid18 = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeSolid_compressed_data, FontAwesomeSolid_compressed_size, 18.f * (2.0f / 3.0f), &FontAwesomeConfig, &IconRanges[0]);
        FontAwesomeSolidBig = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeSolid_compressed_data, FontAwesomeSolid_compressed_size, 30.f * (2.0f / 3.0f), &FontAwesomeConfig, &IconRanges[0]);

        // Sửa lỗi D3DX11 bằng WIC (Windows Imaging Component)
        IWICImagingFactory* wicFactory = nullptr;
        IWICBitmapDecoder* decoder = nullptr;
        IWICBitmapFrameDecode* frame = nullptr;
        IWICFormatConverter* converter = nullptr;

        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

        if (wicFactory) {
            IWICStream* stream = nullptr;
            wicFactory->CreateStream(&stream);
            if (stream) {
                stream->InitializeFromMemory((BYTE*)LogoCirco, sizeof(LogoCirco));
                if (SUCCEEDED(wicFactory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnDemand, &decoder))) {
                    if (SUCCEEDED(decoder->GetFrame(0, &frame))) {
                        wicFactory->CreateFormatConverter(&converter);
                        if (converter) {
                            converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
                            
                            UINT width, height;
                            converter->GetSize(&width, &height);
                            std::vector<BYTE> pixels(width * height * 4);
                            converter->CopyPixels(NULL, width * 4, (UINT)pixels.size(), pixels.data());

                            D3D11_TEXTURE2D_DESC desc = {};
                            desc.Width = width;
                            desc.Height = height;
                            desc.MipLevels = 1;
                            desc.ArraySize = 1;
                            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                            desc.SampleDesc.Count = 1;
                            desc.Usage = D3D11_USAGE_DEFAULT;
                            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

                            D3D11_SUBRESOURCE_DATA subResource = {};
                            subResource.pSysMem = pixels.data();
                            subResource.SysMemPitch = width * 4;

                            ID3D11Texture2D* texture = nullptr;
                            if (SUCCEEDED(Device->CreateTexture2D(&desc, &subResource, &texture))) {
                                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                                srvDesc.Format = desc.Format;
                                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                                srvDesc.Texture2D.MipLevels = 1;
                                Device->CreateShaderResourceView(texture, &srvDesc, &Logo);
                                texture->Release();
                            }
                            converter->Release();
                        }
                        frame->Release();
                    }
                    decoder->Release();
                }
                stream->Release();
            }
            wicFactory->Release();
        }
        CoUninitialize();
    }
}
