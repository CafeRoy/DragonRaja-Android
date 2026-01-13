package com.DragonRefactor; // ★ 注意這裡改成了 org.libsdl.app

import org.libsdl.app.SDLActivity;
import android.os.Bundle;
import android.os.Build;
import android.view.WindowManager;
import android.view.View;
import android.util.Log;

public class MyGameActivity extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // ★ 關鍵：強制設定視窗允許延伸到瀏海/挖孔區域
        if (Build.VERSION.SDK_INT >= 28) {
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                    WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            Log.d("DragonRaja", "Java: ShortEdges mode set successfully!");
        }

        super.onCreate(savedInstanceState);

        // ★ 隱藏導航列
        hideSystemUI();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                        View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_FULLSCREEN
        );
    }
}