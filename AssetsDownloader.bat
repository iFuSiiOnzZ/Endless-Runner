@echo off

SET UNZIP_PATH="%~dp0Output\EndlessRunner"
SET DOWNLOAD_PATH="%TEMP%\assets.zip"

bitsadmin.exe /transfer "Downloading assets..." "https://l3gatq.dm2301.livefilestore.com/y4mFC8ovQ4u0E6QP-khqBHUJfVAtGBbNsYkd6tRBKYmDCTFMvwVPeLv0I97Ay8LXaZVe-nOitYhwZfF-BjIZ506D0RXzChEMag4lEL5NfRXA8FAw-4X8W_eQ3WCDyqml0G0fB8mYHqzHyx8K9W1nExokWIJhwtSbjFvck0OzmT-NxcrgvHGCtc_U0oxr1NjDtxqMf_RqyskGq1kTCw0FqSIHw/assets.zip?download&psid=1" %DOWNLOAD_PATH%
CALL powershell -command Expand-Archive -Force -LiteralPath '%DOWNLOAD_PATH%' -DestinationPath '%UNZIP_PATH%'
DEL %DOWNLOAD_PATH%