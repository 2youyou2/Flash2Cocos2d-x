package akdcl.skeleton.display{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	
	import flash.geom.Matrix;
	
	public class PivotBitmap extends Bitmap{
		public var pivotX:Number;
		public var pivotY:Number;
		
		public var tran_scaleX:Number;
		public var tran_scaleY:Number;
		
		public var tran_skewX:Number;
		public var tran_skewY:Number;
		
		public var tran_x:Number;
		public var tran_y:Number;
		
		private var m:Matrix;
		private var skew_matrix:Matrix;
		
		public function PivotBitmap(bitmapData:BitmapData = null, pixelSnapping:String = "auto", smoothing:Boolean = false){
			super(bitmapData, pixelSnapping, smoothing);
			
			m=new Matrix();
			skew_matrix=new Matrix();
			
			pivotX=0;
			pivotY=0;
			
			tran_scaleX=1;
			tran_scaleY=1;
			
			tran_skewX=0;
			tran_skewY=0;
			
			tran_x=0;
			tran_y=0;
		}
		
		public function update():void{
			m.a = tran_scaleX * Math.cos(tran_skewY)
			m.b = tran_scaleX * Math.sin(tran_skewY)
			m.c = -tran_scaleY * Math.sin(tran_skewX);
			m.d = tran_scaleY * Math.cos(tran_skewX);
			m.tx = tran_x - pivotX;
			m.ty = tran_y - pivotY;
			this.transform.matrix=m;
		}
	}
}