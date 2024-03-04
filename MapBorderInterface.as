package  {
	
	import flash.display.MovieClip;
	import flash.geom.ColorTransform;
	
	
	public class MapBorderInterface extends MovieClip {
		
		public var NorthIndicator:NorthIndic;
		public var CircleBorder:MapBorder;
		public var SquareBorder:MapBorderSq;
		
		public function MapBorderInterface() {
			// constructor code
		}
		
		public function SetBorderType(type:Number) {
			CircleBorder.visible = type == 0;
			SquareBorder.visible = type == 1;
		}
		
		public function SetColor(borderColor:Number, bgColor:Number) {
			NorthIndicator.SetColor(borderColor, bgColor);
			
			CircleBorder.transform.colorTransform = Util.GetColorTransform(borderColor);
			SquareBorder.transform.colorTransform = Util.GetColorTransform(borderColor);
		}
		
		public function SetAlpha(borderAlpha:Number) {
			//NorthIndicator.alpha = borderAlpha;
			CircleBorder.alpha = borderAlpha;
			SquareBorder.alpha = borderAlpha;
		}
	}
	
}
