
<img src="doc/STBoard.png" height="500">

# MIDI2_USB_Dev

- This is an experimental branch to run the MIDI2.0 USB Device on the NUCLEO-F439ZI.
- It converts MIDI1.0 byte messages received via UART into the UMP MIDI2.0 Protocol for output to a USB Host.
- Additionally, it can output UMP received from the USB Host to UART.
- Licensed under the MIT License. It is available for everyone—feel free to fork it for use on various microcontroller boards.

# Hardware Connections

- Connect the NUCLEO-F439ZI's UART5 (Rx: PD2, Tx: PC12) to a 5-Pin(or TRS) MIDI Board connector.
- For easy connection, use the following UART-MIDI converter products:
    - https://www.tindie.com/products/kinoshitalab/midi-uart-interface-san-kit/
    - https://www.switch-science.com/products/8117
- Power from the NUCELO's USB PWR and connect a USB Host to the other USB port (CN13).
- A MIDI2.0 driver-supported OS is required on the host side (macOS 13 or later, Windows 11 Insider Preview).
    - The MIDI1.0 USB driver will not operate correctly.

# How to Build

- Install the VSCode STM32 VS CODE EXTENSION in VSCode:
    - https://www.st.com/content/st_com/ja/campaigns/stm32-vs-code-extension-z11.html
- In the STM32 VS Code Extension, select "Import CMake project" and choose the pulled MIDI2_USB_Dev folder.
- Run "Actions" to open a build-ready VSCode window.
- Connect the PC (with VSCode open) to the NUCLEO, then press the F5 key to run. In Debug mode, the program stops at the main function, so press F5 again to continue.
- Verify UMP transmission and reception data through the Serial Monitor using the ST Link Virtual COM Port.

# Note

- The conversion from UART to MIDI 2.0 Protocol is performed only for certain messages (Note On/Off, CC, Pitch Bend).
- MIDI-CI is not supported. The USB Device merely demonstrates the sending and receiving of MIDI2.0 Protocol UMP.
- Minimal responses are implemented for EndpointDiscovery from the Host. Function Block operations might not function correctly.
- Verified only on Windows 11 Insider Preview / Windows MIDI Services. Operation on macOS is unconfirmed.

# License

- This project is released under the MIT License.
- Note: STM32 HAL drivers are provided by STMicroelectronics and are licensed under their own terms. See the source files for details.

## Included Third-Party Libraries

- This project uses the following third-party libraries as Git submodules:

  - TinyUSB ( https://github.com/hathach/tinyusb )
  - tusb_ump ( https://github.com/midi2-dev/tusb_ump )
  - AM_MIDI2.0Lib ( https://github.com/midi2-dev/AM_MIDI2.0Lib )

- Each submodule retains its original license. Please refer to the respective directories for license details.

# MIDI2_USB_Dev

- MIDI2.0 USB DeviceをNUCLEO-F439ZIで動かすための実験用ブランチです。
- UARTから送られてきたMIDI1.0 Byte MessageをUMP MIDI2.0 Protocolに変換してUSB Hostへ出力します。
- また、USB Hostから送られてきたUMPをUARTに出力することもできます。
- MITライセンスです。誰でも利用可能です。様々なマイコンボードにフォークしてご利用ください。

# ハードウェア接続

- NUCLEO-F439ZIのUART5(Rx:PD2,Tx:PC12)を5PinMIDI(or TRS MIDI)変換ボードに接続してください。
- UART-MIDI変換器はこちらの商品を使うと簡単に接続できます。
  - https://www.tindie.com/products/kinoshitalab/midi-uart-interface-san-kit/
  - https://www.switch-science.com/products/8117
- NUCELOのUSB PWRから給電して、もう片方のUSB端子(CN13)にUSB Hostを接続してください。
- Host側にはMIDI2.0ドライバ対応のOSが必要です。(macOS 13以上、 Windows 11 Insider Preview)
  - MIDI1.0 USB ドライバでは正常動作しません。

# ビルド方法

- VSCode STM32 VS CODE EXTENSIONをVSCodeにインストールします
  - https://www.st.com/content/st_com/ja/campaigns/stm32-vs-code-extension-z11.html
- STM32 VS Code Extension -> "Import CMake project"を選択して、pullしたMIDI2_USB_Devフォルダを指定します
- "Actions"を実行するとVSCodeの画面が開き、ビルドできる状態になります。
- VSCodeを開いているPCとNUCLEOを接続して、F5キーを押すと動作します。Debugモードではmain関数で停止するので、再度F5キーを押してください
- Serial MonitorからUMPの送受信データを確認できます。ST Link Virtual COM Portからご確認ください。

# 注意

- UARTからMIDI2.0 Protocolへの変換は一部のメッセージ(NoteOn/Off,CC,PitchBend)のみ実行します。
- MIDI-CIには対応していません。MIDI2.0 Protocolの送受信をUSB Deviceから確認するだけです。
- HostからのEndpointDiscoveryに対して最低限の応答しか実装していません。Function Block動作については正しく機能しない可能性が高いです。
- Windows 11 Insider Preview / Windows MIDI Servicesでのみ動作確認を行っています。macOSの動作については未確認です。


