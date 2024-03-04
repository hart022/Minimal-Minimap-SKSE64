package  {
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.GraphicsPathCommand;
	
	
	public class FOVView extends MovieClip {
		
		public var FOVBackground:MovieClip;
		public var FOVLineLeft:MovieClip;
		public var FOVLineRight:MovieClip;
		
		private var maskSpr:Sprite;
		
		public function FOVView() {
			// constructor code
		}
		
		public function SetFOV(targetAngle:Number, alphaValue:Number) {
			var maskCommands:Vector.<int> = new Vector.<int>(4, true);
			var maskCoord:Vector.<Number> = new Vector.<Number>(10, true);
			
			var angle:Number = targetAngle;
			var maskSprTemp:Sprite = new Sprite();
			
			FOVLineLeft.rotation = (-targetAngle * 180 / Math.PI) / 2;
			FOVLineRight.rotation = (targetAngle * 180 / Math.PI) / 2;
			
			var sinA:Number = Math.sin(angle / 2);
			var cosA:Number = Math.cos(angle / 2);
			
			maskCommands[0] = GraphicsPathCommand.MOVE_TO;
			maskCommands[1] = GraphicsPathCommand.LINE_TO;
			maskCommands[2] = GraphicsPathCommand.CURVE_TO;
			maskCommands[3] = GraphicsPathCommand.LINE_TO;
			
			maskCoord[0] = 0;
			maskCoord[1] = 0;
			
			maskCoord[2] = -150 * sinA;
			maskCoord[3] = -150 * cosA;
			
			maskCoord[4] = 0;
			maskCoord[5] = -150 / cosA;
			maskCoord[6] = 150 * sinA;
			maskCoord[7] = -150 * cosA;
			
			maskCoord[8] = 0;
			maskCoord[9] = 0;
			
			maskSprTemp.graphics.clear();
			maskSprTemp.graphics.beginFill(0xFFFFFF);
			
			maskSprTemp.graphics.drawPath(maskCommands, maskCoord);
			
			maskSprTemp.graphics.endFill();
			
			this.addChild(maskSprTemp);
			
			if (maskSpr) {
				this.removeChild(maskSpr);
			}
			
			maskSpr = maskSprTemp;
			
			FOVBackground.mask = maskSpr;
			FOVBackground.alpha = alphaValue;
		}
	}
	
}
