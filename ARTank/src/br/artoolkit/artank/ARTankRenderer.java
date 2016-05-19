package br.artoolkit.artank;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import org.artoolkit.ar.base.rendering.ARRenderer;

public class ARTankRenderer extends ARRenderer{

	//Load the native libraries.
    static {
    	System.loadLibrary("c++_shared");
		System.loadLibrary("ARWrapper");
		System.loadLibrary("ARTank");
    }
	
    public static native void surfaceCreated();
    public static native void surfaceChanged(int w, int h);
    public static native void drawFrame(); 
	
	@Override
	public void onSurfaceChanged(GL10 gl, int w, int h) {
		// TODO Auto-generated method stub
		super.onSurfaceChanged(gl, w, h);
		ARTankRenderer.surfaceChanged(w, h);
	}
	
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		super.onSurfaceCreated(gl, config);
		ARTankRenderer.surfaceCreated();
	}
	
	@Override
	public void draw(GL10 gl) {
		// TODO Auto-generated method stub
		ARTankRenderer.drawFrame();
	}
}
