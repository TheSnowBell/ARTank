package br.artoolkit.artank;

import org.artoolkit.ar.base.ARActivity;
import org.artoolkit.ar.base.rendering.ARRenderer;

import android.content.Context;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.LinearLayout;
	
public class ARTank extends ARActivity implements OnClickListener{
	
	private ARTankRenderer arTankRenderer = new ARTankRenderer();
	
	private FrameLayout mainLayout;
	private ImageButton buttonShot;
	private ImageButton buttonUp;
	private ImageButton buttonDown;
	private ImageButton buttonReset;
	
	public static native void shutdown();
	public static native void shot();
	public static native void cannonUp();
	public static native void cannonDown();
	public static native void reset();
	
	
	private ImageButton createButton(Context context, int id, int resID){
		ImageButton imageButton = new ImageButton(context);
		imageButton.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
		imageButton.setAlpha(0.5f);
		imageButton.setId(id);
		imageButton.setImageResource(resID);
		return imageButton;
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
	}
	
	@Override
	public void onResume() {
		super.onResume();
		
		mainLayout = (FrameLayout)this.findViewById(R.id.mainLayout);
		buttonUp = createButton(this, 0, R.drawable.arrow_up);
		buttonDown = createButton(this, 1, R.drawable.arrow_down);
		buttonShot = createButton(this, 2, R.drawable.center_focus);
		buttonReset = createButton(this, 3, R.drawable.ic_refresh);
		
		buttonUp.setOnClickListener(this);
		buttonDown.setOnClickListener(this);
		buttonShot.setOnClickListener(this);
		buttonReset.setOnClickListener(this);
		
		LinearLayout lineLayout = new LinearLayout(this);
 		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
 		params.gravity = Gravity.BOTTOM;
 		lineLayout.setLayoutParams(params);
 		lineLayout.setOrientation(LinearLayout.VERTICAL);
 		
 		lineLayout.addView(buttonUp);
 		lineLayout.addView(buttonDown);
		
 		mainLayout.addView(lineLayout);
 		mainLayout.addView(buttonShot, new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, Gravity.END | Gravity.BOTTOM ));
 		mainLayout.addView(buttonReset, new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, Gravity.END | Gravity.TOP ));
		
	}
	
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		ARTank.shutdown();
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}
	
	@Override
	protected ARRenderer supplyRenderer() {
		// TODO Auto-generated method stub
		return arTankRenderer;
	}

	@Override
	protected FrameLayout supplyFrameLayout() {
		// TODO Auto-generated method stub
		return (FrameLayout) this.findViewById(R.id.mainLayout);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case 0:
			ARTank.cannonUp();
		break;

		case 1:
			ARTank.cannonDown();
		break;
		
		case 2:
			ARTank.shot();
		break;
		
		case 3:
			ARTank.reset();
		break;
		}
		
	}

	
}
