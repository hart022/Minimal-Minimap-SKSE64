package  {
	import flash.geom.ColorTransform;
	
	public class Util {

		public function Util() {
			// constructor code
			
			
		}
			
		static public function GetColorTransform(targetColor:Number) : ColorTransform {
			var cTrans:ColorTransform = new ColorTransform();
			
			cTrans.color = targetColor;
			
			cTrans.redMultiplier = cTrans.redOffset / 255;
			cTrans.greenMultiplier = cTrans.greenOffset / 255;
			cTrans.blueMultiplier = cTrans.blueOffset / 255;
			
			cTrans.redOffset = 0;
			cTrans.greenOffset = 0;
			cTrans.blueOffset = 0;
			
			return cTrans;
		}

	}
	
}
