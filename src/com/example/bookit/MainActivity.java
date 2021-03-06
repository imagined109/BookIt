package com.example.bookit;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvException;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;

import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.CompressFormat;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

	
	private static final String TAG = "BookIt";
	
	Preview mPreview; 
	ResultView mResultView;
	private Context mContext = this; 
		
	private final static String INPUT_IMG_FILENAME = "/Documents/img1.jpg"; 
	private final static String INPUT_IMG_PATH = Environment.getExternalStorageDirectory().toString() + INPUT_IMG_FILENAME;
	
	//flag to check if camera is ready for capture
	private boolean mCameraReadyFlag = true;
	
	 private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
	        @Override
	        public void onManagerConnected(int status) {
	            switch (status) {
	                case LoaderCallbackInterface.SUCCESS:
	                {
	                    Log.i(TAG, "OpenCV loaded successfully");
//	                    mOpenCvCameraView.enableView();
	                } break;
	                default: 
	                {
	                    super.onManagerConnected(status);
	                } break;
	            }
	        }
	    };
	
    // Called when the activity is first created. 
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        
        //make the screen full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
    			WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //remove the title bar
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        mResultView=new ResultView(this);
        mPreview = new Preview(this);
        
        //set Content View as the preview
        setContentView(mPreview);
        
        //add result view  to the content View
        addContentView(mResultView,new LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT));
        
        //set the orientation as landscape
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);      
        
        // Load opencv
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_8, this, mLoaderCallback);
    }
	
	
	
  //onKeyDown is used to monitor button pressed and facilitate the switching of views
    @Override
    public boolean onKeyDown(int keycode,KeyEvent event)
    {
  	 //check if the camera button is pressed
  	 if(keycode==KeyEvent.KEYCODE_CAMERA)
  	 {
  		 //if result
  		 if (mResultView.IsShowingResult)
  		 {
  			mResultView.IsShowingResult = false;
  		 }
  		 else if (mCameraReadyFlag == true)//switch to camera view
  		 {
  			mCameraReadyFlag = false;
  			mPreview.camera.takePicture(shutterCallback, rawCallback, jpegCallback);
  		 }
  		 return true;
  	 }
  	 return super.onKeyDown(keycode, event);
    }
    @Override
    public boolean onTouchEvent(MotionEvent event) /** When the user touches the screen, run sift **/
    {
	  if (event.getAction() == MotionEvent.ACTION_DOWN) {  // finger touches the screen
		 //if result
  		 if (mResultView.IsShowingResult)
  		 {
  			mResultView.IsShowingResult = false;
  			mPreview.camera.startPreview();  
  		 }
  		 else if (mCameraReadyFlag == true)//switch to camera view
  		 {
  			mCameraReadyFlag = false;  			
  			mPreview.camera.takePicture(shutterCallback, rawCallback, jpegCallback);
  			

			
			
  		 }
	  }
	  return true;
   }
	
	
	

    // Called when shutter is opened
    ShutterCallback shutterCallback = new ShutterCallback() { 
      public void onShutter() {      
      }
    };
 
    // Handles data for raw picture  
    PictureCallback rawCallback = new PictureCallback() { 
   	@Override
	public void onPictureTaken(byte[] arg0, android.hardware.Camera arg1) {
	}
    };
    
    //store the image as a jpeg image
    public  boolean compressByteImage(Context mContext, byte[] imageData,
  			int quality) {
    	File sdCard = Environment.getExternalStorageDirectory();
  		FileOutputStream fileOutputStream = null;

  		try {  			 		 	
  			BitmapFactory.Options options=new BitmapFactory.Options();
  			options.inSampleSize = 4;     //DOWNSAMPLE THE IMAGE SO IT WRITES FASTER		
  			Bitmap myImage = BitmapFactory.decodeByteArray(imageData, 0,
  					imageData.length,options);  			
  			fileOutputStream = new FileOutputStream(
  					sdCard.toString() +INPUT_IMG_FILENAME);							
 
  			Log.e(TAG, sdCard.toString() +INPUT_IMG_FILENAME); 
  			BufferedOutputStream bos = new BufferedOutputStream(
  					fileOutputStream); 
  			 
  			//compress image to jpeg
  			myImage.compress(CompressFormat.JPEG, quality, bos);

  			bos.flush();
  			bos.close();  			 
  			fileOutputStream.close();  			

  		} catch (FileNotFoundException e) {
   			Log.e(TAG, "FileNotFoundException");
  			e.printStackTrace();
  		} catch (IOException e) {
  			Log.e(TAG, "IOException");
  			e.printStackTrace();
  		}
  		return true;
  	}
    
 // Handles data for jpeg picture
    PictureCallback jpegCallback = new PictureCallback() { 
	@Override
	public void onPictureTaken(byte[] imageData, android.hardware.Camera camera) {
			if (imageData != null) {
				
  				Intent mIntent = new Intent();
  				//compress image  
  				compressByteImage(mContext, imageData, 75);  				
  				setResult(0, mIntent);
  				
  				// RUN SIFT ON THE SAVED PICTURE
  				  				
  				Similarity task = new Similarity();
  				task.execute();
  				
  				//start the camera view again .
  				  
//  				camera.startPreview();  				
  			}		
	}
    };
       
    
	//******************************************************************************* 
	// Get the similarity of the image
	//*******************************************************************************
	
	public class Similarity  extends AsyncTask<String, Integer , Void>
	{
		public Mat img;
		
		/** Before running the sifts, load the image **/
	    protected void onPreExecute() {
	    	mPreview.camera.stopPreview();  
	    	// Read the image 
	    	img = Highgui.imread(INPUT_IMG_PATH,Highgui.CV_LOAD_IMAGE_COLOR);
	    	
			if (img.empty()){
				Log.e(TAG, "Did not read image!");
			} else {
				//Log.e(TAG, "Image Read!!!!!!!!!!");
			}
	    }
		
		@Override
		protected Void doInBackground(String... params) { 
			
			// Copy the image to build the output on.
			Mat results = new Mat(img.height(),img.width(),CvType.CV_8U, new Scalar(4));
			
			// Get the filenames of all the book covers (in the form isbn.jpg)
  			
  			String path = Environment.getExternalStorageDirectory().toString()+"/Documents/Books";
  			//Log.d("Files", "Path: " + path);
  			File f = new File(path);        
  			//File file[] = f.listFiles();
  			String bookCovers[] = f.list();      
  			

  			Log.e(TAG, "Start Runnnig C++ Code");
			// Run the sifts - NOTE: BECAUSE C USES POINTERS IT FILLS THE RESULTS MATRIX WITH THE CORRECT VALUES WITHOUT RETURNING IT!
			String Link = NonfreeJNILib.runSift(img.getNativeObjAddr(),results.getNativeObjAddr(),bookCovers);
			Log.e(TAG, "Done Runnnig C++ Code");  
			Log.e(TAG, Link); 
			
			if(Link != null && !Link.isEmpty()){
				// Load the website associated with the found book.
				Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(Link));
				startActivity(browserIntent);
			}
			 
			// Set the result image  
			mResultView.resultImage = null;   
			try {
			    mResultView.resultImage = Bitmap.createBitmap(results.cols(), results.rows(), Bitmap.Config.ARGB_8888);
			    Utils.matToBitmap(results,  mResultView.resultImage);
			    mResultView.IsShowingResult = true;	
			} 
			catch (CvException e){Log.d("Exception",e.getMessage());}	 
			
			/////////////////////TEMP//////////////////TEMP///////////////////////////
//			try {  		 	 			
//	  			  
//				FileOutputStream fileOutputStream = new FileOutputStream("/sdcard/Documents/img1_result.jpg");							
//	  			BufferedOutputStream bos = new BufferedOutputStream(fileOutputStream); 
//	  			  
//	  			//compress image to jpeg
//	  			mResultView.resultImage.compress(CompressFormat.JPEG, 100, bos);
//  
//	  			bos.flush();   
//	  			bos.close();  		 
//	  			fileOutputStream.close();  			
//  
//	  		} catch (FileNotFoundException e) { 
//	   			Log.e(TAG, "FileNotFoundException");
//	  			e.printStackTrace();
//	  		} catch (IOException e) {
//	  			Log.e(TAG, "IOException");
//	  			e.printStackTrace();
//	  		}   
			
						
			//release camera when previous image is processed 
			mCameraReadyFlag = true;   
			return null; 
		}
	 
	
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	Button runBtn; 
	TextView txtView; 
	 
	 
}
