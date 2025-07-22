package org.coco;

import android.app.Activity;
import android.app.ActionBar;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.content.Context;
import android.content.res.Resources;
import android.view.MotionEvent;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

public class MainActivity extends Activity {
    public static native void start(Resources res);
    public static native void update(Resources res);
    public static native void render(Resources res);

    public static native void size(Resources res, int width, int height);

    public static native void motion(float x, float y, int action, int pointerCount, int pointerID);

    public class RendererImpl implements Renderer {
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            start(getResources());
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            size(getResources(), width, height);
        }

        public void onDrawFrame(GL10 gl) {
            update(getResources());
            render(getResources());
        }
    }

    public class ViewImpl extends GLSurfaceView {
        public RendererImpl renderer;

        public ViewImpl(Context context) {
            super(context);

            setEGLConfigChooser(8, 8, 8, 8, 24, 8);
            setEGLContextClientVersion(3);

            renderer = new RendererImpl();
            setRenderer(renderer);
        }
    }

    public ViewImpl view;

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        for(int i = 0; i < event.getPointerCount(); i++) {
            motion(event.getRawX(i), event.getRawY(i), event.getAction(), event.getPointerCount(), i);
        }
        return false;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        System.loadLibrary("coco");

        view = new ViewImpl(this);
        setContentView(view);

        ActionBar actionBar = getActionBar();
        if(actionBar != null) {
            actionBar.hide();
        }
    }
}