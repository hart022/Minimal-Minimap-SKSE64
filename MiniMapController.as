package  {
	
	import flash.display.MovieClip;
    import flash.geom.ColorTransform;
	import fl.transitions.Transition;
	import fl.transitions.TransitionManager;
	import fl.transitions.Rotate;
	import fl.transitions.easing.None;
	import flash.events.Event;
	import flash.display.Loader;
	import flash.net.URLRequest;
	import flash.display.DisplayObject;
	import flash.events.IOErrorEvent;
	import flash.system.LoaderContext;
	import flash.system.ApplicationDomain;
	import F4SE.ICodeObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	
	
	public class MiniMapController extends MovieClip implements ICodeObject {
		
		private var SymbolArray:Array = [ SymbolCircle, SymbolSquere, SymbolDiamond, SymbolTriangle, SymbolCross, SymbolStar, SymbolBang ];
		
		private var DataArray:Array = [];
		
		private var PlacedArray:Array = [];
		
		private var anchor:Number;
		private var xpos:Number;
		private var ypos:Number;
		private var bgScale:Number;
		private var bgOpacity:Number;
		private var symbolSize:Number;
		private var doSonar:Boolean;
		private var useOffsetZ:Boolean;
		private var rotateGrid:Boolean;
		private var hideOffsetZ:Boolean;
		private var fovEnable:Boolean;
		private var hideIndicator:Boolean;
		private var sonarAmount:Number;
		private var offsetZAmount:Number;
		private var gridType:Number;
		private var gridAlpha:Number;
		private var interfaceType:Number;
		private var localMapOpacity:Number;
		private var fovOpacity:Number;
		
		private var borderOpacity:Number;
		private var radarOpacity:Number;
		private var playerOpacity:Number;
		
		private var playerScale:Number;
		
		private var borderColor:Number;
		private var bgColor:Number;
		private var gridColor:Number;
		private var localMapColor:Number;
		private var sonarColor:Number;
		private var fovColor:Number;
		private var playerColor:Number;
		
		private var targetScale:Number = 1.0;
		
		private var targetImage:String;
		
		private var imgLoader:Loader;
		private var imgObject:DisplayObject;
		private var imgLoaded:Boolean;
		
		private const typeCount:Number = 11;
		private const typeOptCount:Number = 4;
		
		public var CircleInstance:CircleMinimap;
		public var SquareInstance:SquereMinimap;
		
		private var NowInstance:MovieClip;
		private var MapViewport:LocalMapViewport;
		private var MinimapBackground:MovieClip;
		private var MinimapBorder:MapBorderInterface;
		private var PlayerMark:MovieClip;
		private var SonarBar:MovieClip;
		private var BGCircle:MovieClip;
		private var BGSquere:MovieClip;
		private var FOVViewport:FOVView;
		
		public var F4SEPlugin:Object;
		public var F4SEBase:Object;
		
		public function MiniMapController() {
			super();
			this.visible = true;
			
			imgLoader = new Loader();
			imgLoader.contentLoaderInfo.addEventListener(Event.COMPLETE, completeHandler);
			imgLoader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			
			imgLoaded = false;
			imgLoader.load(new URLRequest("img://MinimapImage"), new LoaderContext(false,ApplicationDomain.currentDomain));
			
			//this.transform.colorTransform = Util.GetColorTransform(0xFF8000);
		}
        
        public function onF4SEObjCreated(e:*) : void {
			F4SEBase = e;
            F4SEPlugin = e.plugins.MinimalMinimap;
        }
		
		public function UpdateConfig(arr:Array) {
			DataArray = arr.slice(0, typeCount * typeOptCount);
			var ConfArray:Array = arr.slice(typeCount * typeOptCount);
			anchor = ConfArray[0] as Number;
			xpos = ConfArray[1] as Number;
			ypos = ConfArray[2] as Number;
			bgScale = ConfArray[3] as Number;
			symbolSize = ConfArray[4] as Number;
			doSonar = (ConfArray[5] as Number) != 0;
			sonarAmount = ConfArray[6] as Number;
			bgOpacity = ConfArray[7] as Number;
			useOffsetZ = (ConfArray[8] as Number) != 0;
			offsetZAmount = ConfArray[9] as Number;
			gridType = ConfArray[10] as Number;
			rotateGrid = (ConfArray[11] as Number) != 0;
			gridAlpha = ConfArray[12] as Number;
			hideOffsetZ = (ConfArray[13] as Number) != 0;
			interfaceType = ConfArray[14] as Number;
			localMapOpacity = ConfArray[15] as Number;
			fovEnable = (ConfArray[16] as Number) != 0;
			fovOpacity = ConfArray[17] as Number;
			hideIndicator = (ConfArray[18] as Number) != 0;
			
			borderColor = ConfArray[19] as Number;
			bgColor = ConfArray[20] as Number;
			gridColor = ConfArray[21] as Number;
			localMapColor = ConfArray[22] as Number;
			sonarColor = ConfArray[23] as Number;
			fovColor = ConfArray[24] as Number;
			playerColor = ConfArray[25] as Number;
			
			borderOpacity = ConfArray[26] as Number;
			radarOpacity = ConfArray[27] as Number;
			playerOpacity = ConfArray[28] as Number;
			playerScale = ConfArray[29] as Number;
			
			if (interfaceType == 0) {
				CircleInstance.visible = true;
				SquareInstance.visible = false;
				
				MapViewport = CircleInstance.MapViewport;
				
				NowInstance = CircleInstance;
			}
			else if (interfaceType == 1) {
				CircleInstance.visible = false;
				SquareInstance.visible = true;
				
				MapViewport = SquareInstance.MapViewport;
				
				NowInstance = SquareInstance;
			}
			else {
				CircleInstance.visible = false;
				SquareInstance.visible = false;
				
				NowInstance = null;
			}
			
			if (NowInstance) {
				MinimapBackground = NowInstance.MinimapBackground;
				MinimapBorder = NowInstance.MinimapBorder;
				PlayerMark = NowInstance.PlayerMark;
				SonarBar = NowInstance.SonarBar;
				BGCircle = NowInstance.BGCircle;
				BGSquere = NowInstance.BGSquere;
				FOVViewport = NowInstance.FOVViewport;
				MinimapBorder.SetBorderType(interfaceType);
			}
			
			FOVViewport.visible = fovEnable;
			
			MinimapBorder.SetColor(borderColor, bgColor);
			PlayerMark.transform.colorTransform = Util.GetColorTransform(playerColor);
			MinimapBackground.transform.colorTransform = Util.GetColorTransform(bgColor);
			BGCircle.transform.colorTransform = BGSquere.transform.colorTransform = Util.GetColorTransform(gridColor);
			MapViewport.transform.colorTransform = Util.GetColorTransform(localMapColor);
			SonarBar.transform.colorTransform = Util.GetColorTransform(sonarColor);
			FOVViewport.transform.colorTransform = Util.GetColorTransform(fovColor);
			
			MinimapBorder.NorthIndicator.visible = !hideIndicator;
			
			if (gridType == 1) {
				BGSquere.visible = true;
				BGCircle.visible = false;
			}
			else if (gridType == 2) {
				BGSquere.visible = false;
				BGCircle.visible = true;
			}
			else if (gridType == 3) {
				BGSquere.visible = true;
				BGCircle.visible = true;
			}
			else {
				BGSquere.visible = false;
				BGCircle.visible = false;
			}
			
			if (doSonar) {
				TransitionManager.start(SonarBar, {type:Rotate, direction:Transition.IN, duration:86400, easing:None.easeNone, ccw:true, degrees:86400 * 15 * sonarAmount});
			}
			
			MinimapBackground.alpha = bgOpacity;
			BGSquere.alpha = BGCircle.alpha = gridAlpha;
			MapViewport.alpha = localMapOpacity;
			PlayerMark.alpha = playerOpacity;
			SonarBar.alpha = radarOpacity;
			MinimapBorder.SetAlpha(borderOpacity);
			
			NowInstance.scaleX = bgScale;
			NowInstance.scaleY = bgScale;
			
			var moddedWidth:Number = 155 * bgScale;
			var moddedHeight:Number = 155 * bgScale;
			
			if (anchor == 0) {
				NowInstance.x = xpos + moddedWidth;
				NowInstance.y = ypos + moddedHeight;
			}
			else if (anchor == 1) {
				NowInstance.x = 1280 - xpos - moddedWidth;
				NowInstance.y = ypos + moddedHeight;
			}
			else if (anchor == 2) {
				NowInstance.x = xpos + moddedWidth;
				NowInstance.y = 720 - ypos - moddedHeight;
			}
			else if (anchor == 3) {
				NowInstance.x = 1280 - xpos - moddedWidth;
				NowInstance.y = 720 - ypos - moddedHeight;
			}
		}
		
		public function UpdateMinimap(arr:Array) {
			var i:int = 0;
			var mc:MovieClip;
			var mc2:MovieClip;
			
			for (i = 0; i < PlacedArray.length; i++) {
				mc = PlacedArray[i] as MovieClip;
				MinimapBorder.removeChild(mc);
			}
			
			SonarBar.visible = doSonar;
			
			PlacedArray = [];
			
			this.scaleX = 1.0;
			this.scaleY = 1.0;
			this.x = 0.0;
			this.y = 0.0;
			
			var plAngle:Number = arr.shift() as Number;
			var mapRotation:Boolean = arr.shift() as Boolean;
			
			var camAngle:Number = arr.shift() as Number;
			var useCamRotation:Boolean = arr.shift() as Boolean;
			
			var camFOV:Number = arr.shift() as Number;
			var nowZoomLevel:Number = arr.shift() as Number;
			
			PlayerMark.scaleX = playerScale / nowZoomLevel;
			PlayerMark.scaleY = playerScale / nowZoomLevel;
			
			if (mapRotation && interfaceType == 0) {
				if (useCamRotation) {
					MinimapBorder.rotation = -camAngle;
					MapViewport.rotation = -camAngle;
					PlayerMark.rotation = plAngle - camAngle;
					FOVViewport.rotation = 0;
				}
				else {
					MinimapBorder.rotation = -plAngle;
					MapViewport.rotation = -plAngle;
					PlayerMark.rotation = 0;
					FOVViewport.rotation = camAngle - plAngle;
				}
			}
			else {
				MinimapBorder.rotation = 0;
				MapViewport.rotation = 0;
				PlayerMark.rotation = plAngle;
				FOVViewport.rotation = camAngle;
			}
			
			if (fovEnable) {
				FOVViewport.SetFOV(camFOV, fovOpacity);
			}
			
			if (rotateGrid) {
				BGSquere.rotation = BGCircle.rotation = MinimapBorder.rotation
			}
			else {
				BGSquere.rotation = BGCircle.rotation = 0;
			}
			
			MapViewport.UpdatePos();
			
			arr.sort(sortByPriority);
			
			var renderRadius:Number = F4SEPlugin.Radius();
			
			for (i = 0; i < arr.length; i++) {
				var obj:Object = arr[i] as Object;
				if (obj["type"]) {
					var obj_type:Number = (obj["type"] as Number) - 1;
					var sym_type:Number = DataArray[obj_type] as Number;
					var offsetx:Number = obj["offsetx"] as Number;
					var offsety:Number = obj["offsety"] as Number;
					var offsetz:Number = obj["offsetz"] as Number;
					var over:Boolean = obj["over"] as Boolean;
					var zTest:Boolean = Math.abs(offsetz) >= offsetZAmount;
					
					if (zTest && hideOffsetZ) {
						continue;
					}
					
					var targetSymbol:Class = SymbolArray[sym_type] as Class;
					var targetOpacity:Number = DataArray[obj_type + typeCount] as Number;
					var targetColor:uint = DataArray[obj_type + (typeCount * 2)] as uint;
					var targetPriority:Number = DataArray[obj_type + (typeCount * 3)] as Number;
					
					if (interfaceType == 0)
					{
						if (Math.pow(offsetx, 2) + Math.pow(offsety, 2) > 1 && !over) {
							continue;
						}
					}
					else if (interfaceType == 1)
					{
						offsetx *= 1.4;
						offsety *= 1.4;
						var offax:Number = Math.abs(offsetx);
						var offay:Number = Math.abs(offsety);
						if ((offax > 1 || offay > 1) && !over) {
							continue;
						}
						if (offax > 1.05) {
							offsety *= 1.05 / offax;
							offsetx *= 1.05 / offax;
						}
						else if (offay > 1.05) {
							offsety *= 1.05 / offay;
							offsetx *= 1.05 / offay;
						}
					}
					
					mc = new targetSymbol() as MovieClip;
					MinimapBorder.addChild(mc);
					mc.x = (offsetx * renderRadius);
					mc.y = (offsety * renderRadius);
					mc.z = -targetPriority;
					mc.scaleX = symbolSize;
					mc.scaleY = symbolSize;
					mc.rotation = -MinimapBorder.rotation;
					if (useOffsetZ && zTest) {
						if (offsetz > 0) {
							mc2 = new SymbolUpper();
						}
						else {
							mc2 = new SymbolLower();
						}
						mc2.y = -11;
						mc.addChild(mc2);
					}
					var cTrans:ColorTransform = new ColorTransform();
					cTrans.color = targetColor;
					cTrans.alphaMultiplier = targetOpacity;
					mc.transform.colorTransform = cTrans;
					PlacedArray.push(mc);
				}
			}
		}
		
		public function UpdateVisible(visibility:Boolean) {
			this.visible = visibility;
		}
		
		public function UpdateImage(scale:Number, modX:Number, modY:Number) {
			targetScale = scale;
			MapViewport.SetPos(modX, modY);
			if (imgLoaded) {
				imgLoaded = false;
				imgLoader.close();
				imgLoader.unloadAndStop(true);
				imgLoader.load(new URLRequest("img://MinimapImage"), new LoaderContext(false,ApplicationDomain.currentDomain));
			}
		}

		function completeHandler(event:Event):void {
			if (MapViewport) {
				MapViewport.SetMapImage(event.currentTarget.content, targetScale);
				MapViewport.SetMapMask(interfaceType);
				MapViewport.UpdatePos();
			}
			
			imgLoaded = true;
		}
		
		function ioErrorHandler(event:IOErrorEvent):void {
			if (MapViewport) {
				MapViewport.SetMapImage(null, 0.0);
				MapViewport.SetMapMask(-1);
			}
			
			imgObject = null;
			imgLoaded = true;
		}
		
		function sortByPriority(obj:Object, obj2:Object) : Number {
			var obj_type:Number = (obj["type"] as Number) - 1;
			var obj_type2:Number = (obj2["type"] as Number) - 1;
			if (obj_type == obj_type2) {
				return 0;
			}
			var pri:Number = DataArray[obj_type + (typeCount * 3)];
			var pri2:Number = DataArray[obj_type2 + (typeCount * 3)];
			if (pri != pri2) {
				return pri - pri2;
			}
			return obj_type - obj_type2;
		}
	}
	
}
