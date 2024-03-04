package  {
	
	import flash.display.MovieClip;
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	
	
	public class LocalMapViewport extends MovieClip {
		
		public var LocalMapHolder:DisplayObject;
		public var MaskHolder:Sprite;
		
		private var localX:Number;
		private var localY:Number;
		
		private var baseScale:Number;
		private var baseWidth:Number;
		private var baseHeight:Number;
		
		public function LocalMapViewport() {
			// constructor code
		}
		
		public function SetMapImage(imgObject:DisplayObject, scale:Number) {
			if (LocalMapHolder) {
				this.removeChild(LocalMapHolder);
				LocalMapHolder = null;
			}
			
			if (imgObject && scale > 0) {
				baseScale = scale * 1280 / imgObject.width;
				baseWidth = imgObject.width;
				baseHeight = imgObject.height;
				
				this.addChild(imgObject);
				
				imgObject.cacheAsBitmap = true;
				
				LocalMapHolder = imgObject;
				LocalMapHolder.x = -(baseWidth * baseScale / 2) + localX;
				LocalMapHolder.y = -(baseHeight * baseScale / 2) - localY;
				LocalMapHolder.scaleX = baseScale;
				LocalMapHolder.scaleY = baseScale;
			}
		}
		
		public function SetMapMask(type:Number) {
			if (MaskHolder) {
				this.removeChild(MaskHolder);
				MaskHolder = null;
			}
			
			if (LocalMapHolder) {
				MaskHolder = new Sprite();
				this.addChild(MaskHolder);
					
				MaskHolder.graphics.beginFill(0xFFFFFF); 
				
				if (type == 0)
				{
					this.scaleX = 1.0;
					this.scaleY = 1.0;
					MaskHolder.graphics.drawCircle(0, 0, 150); 
				}
				else if (type == 1)
				{
					this.scaleX = 1.4;
					this.scaleY = 1.4;
					MaskHolder.graphics.drawRect(-150 / 1.4, -150 / 1.4, 300 / 1.4, 300 / 1.4);
				}
				
				MaskHolder.graphics.endFill();
			
				LocalMapHolder.mask = MaskHolder;
			}
		}
		
		public function SetPos(modX:Number, modY:Number) {
			localX = modX;
			localY = modY;
		}
		
		public function UpdatePos() {
			if (LocalMapHolder) {
				LocalMapHolder.x = -(baseWidth * baseScale / 2) + localX;
				LocalMapHolder.y = -(baseHeight * baseScale / 2) - localY;
			}
		}
	}
	
}
