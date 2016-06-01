package br.artoolkit.artank;

import android.util.Log;


public class Simulation implements Runnable{

	public static native void shutdown();
	public static native void shot();
	public static native void _cannonUp();
	public static native void _cannonDown();
	public static native void reset();
	public static native void initialise();
	public static native void running();


	private boolean finished;

	public Simulation() {
		//mPaused = false;
		finished = false;
		Simulation.initialise();
	}
	
	public void Shutdown(){
		shutdown();
	}
	
	public void cannonUp(){
		_cannonUp();
	}
	
	public void cannonDown(){
		_cannonDown();
	}
	
	public void Shot(){
		shot();
	}
	
	public void Reset(){
		reset();
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		long start = System.currentTimeMillis();
		long elapsedTime;	
		int frame = 0;
		long frameRate = 1000/30;
		long atual, anterior;
		
		while(!finished && !Thread.interrupted()) {
			
			anterior = System.currentTimeMillis();
			Simulation.running();
			atual = System.currentTimeMillis();
			frame++;
			
			elapsedTime = atual - anterior;
			try {
				if(elapsedTime < frameRate){
					Thread.sleep(frameRate - elapsedTime);
				}
			} catch (InterruptedException e) {
				// TODO: handle exception
				break;
			}
			
			if((atual - start) >= 1000l){
				Log.i("Frame", "start: "+ start +"\n"+ "end: "+ atual +"\n" +"Frames: "+frame+"\n");
				frame = 0;
				start = atual;
			}
		}
	}
	
	public boolean isFinished() {
		return finished;
	}

	public void setFinished(boolean mFinished) {
		this.finished = mFinished;
	}
	
	
}
