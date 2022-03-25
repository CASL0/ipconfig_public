# ipconfig_public
ローカルIPアドレス、グローバルIPアドレスの情報を表示するアプリケーションです。

## 開発
### Windows版
#### 開発環境
* C++ 17
* Visual Studio 2019
  * MFC

#### ビルド手順
1. `win/ipconfig_public.sln`をVisualStudioで開く。
2. アーキテクチャをx86に設定する。
3. `ビルド`>`ソリューションのビルド`をクリックする。
4. ビルド終了後ipconfig_public.exeが生成される。

### Android版
#### 開発環境
* Android Studio 2021.1.1
* kotlin 1.6.10
* Android Gradle Plugin 7.1.2
* Gradle 7.2

#### ビルド手順
1. cd android
1. ./gradlew build

## ドキュメント
* [docs](/docs)