# Frillshark  

このプログラムは、C++ での 「ODBC」接続を補助します。  
This program assists in connecting to ODBC For C++  
Use gtkmm3.0 Lunux GUI Application
  
## 01. Configuration  
 必要パッケージ  
 ODBC の セットアップ  
   
 Microsoft のODBCページを参照の上セットアップ、  
または各ディストリビューション標準のセットアップ  
libodbc.so,環境ファイル（odbc.ini,odbcinst.ini）をセットアップ  

gtk3.0,gtkmm3.0  
  
## 02. SetUp
$ ./bld.sh  

## 03. Execute (Frillshark)  

Driver:選択指がなければセットアップをやり直してください。  
server:サーバー名かIPアドレスを入力してください。  
User ID:ユーザー名  
Password:パスワード  
Database:C++のclassを作成するデータベース  
[OK]ボタン   
ボタンをクリックするとファイル保存用ダイアログが表示されますので
ファイル名を入力してください。  

