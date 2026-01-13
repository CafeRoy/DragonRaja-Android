# ==========================================
# DragonRaja Android - 一鍵上傳腳本
# ==========================================

# 設定文字顏色方便閱讀
$host.UI.RawUI.WindowTitle = "Android Code Auto Push"
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "      準備上傳 Android 代碼至 GitHub      " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

# 1. 顯示目前更動的檔案
Write-Host "Checking status..." -ForegroundColor Yellow
git status
Write-Host ""

# 2. 詢問提交訊息 (如果懶得打字，直接按 Enter 會自動填入時間)
$inputMsg = Read-Host "請輸入 Commit 訊息 (直接按 Enter 將使用當前時間)"

if ([string]::IsNullOrWhiteSpace($inputMsg)) {
    $date = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $commitMsg = "Auto Update: $date"
    Write-Host "使用自動訊息: $commitMsg" -ForegroundColor Gray
} else {
    $commitMsg = $inputMsg
}

# 3. 執行 Git 三部曲
Write-Host "1. Adding files..." -ForegroundColor Green
git add .

Write-Host "2. Committing..." -ForegroundColor Green
git commit -m "$commitMsg"

Write-Host "3. Pushing to GitHub..." -ForegroundColor Green
# 注意：這裡指定推送到 main 分支
git push origin main

# 4. 結束確認
if ($?) {
    Write-Host ""
    Write-Host "✅ 成功！代碼已安全備份到 GitHub。" -ForegroundColor Cyan
} else {
    Write-Host ""
    Write-Host "❌ 上傳失敗，請檢查錯誤訊息 (可能是網路問題或需要 Pull)。" -ForegroundColor Red
}

Write-Host ""
Pause