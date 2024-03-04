package  {
	
	import flash.display.MovieClip;
	import flash.geom.ColorTransform;
	
	
	public class NorthIndic extends MovieClip {
		
		public var Letter:MovieClip;
		public var LetterBG:MovieClip;
		
		public function NorthIndic() {
			// constructor code
		}
		
		public function SetColor(borderColor:Number, bgColor:Number) {
			Letter.transform.colorTransform = Util.GetColorTransform(borderColor);
			LetterBG.transform.colorTransform = Util.GetColorTransform(bgColor);
		}
	}
	
}
