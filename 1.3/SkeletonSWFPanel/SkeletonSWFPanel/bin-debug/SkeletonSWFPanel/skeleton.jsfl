var dom = fl.getDocumentDOM();
var library = dom?dom.library:null;
var time = getTimer();

function trace(){
	var _str = "";
	for(var _i = 0;_i < arguments.length;_i ++){
		if(_i!=0){
			_str += ", ";
		}
		_str += arguments[_i];
	}
	fl.trace(_str);
}

function getTimer(){
	return new Date().getTime();
}

function checkTime(_id){
	if(_id){
		trace(_id+"，耗时："+(getTimer()-time)+" 毫秒。");
	}
	time = getTimer();
	return time;
}


function errorDOM(){
	if(!dom){
		alert("没有打开的 FLA 档案！");
		return true;
	}
	return false;
}

//dr默认为1
//x y r skXY 默认为 0
//scXY 默认为 1

var SKELETON = "skeleton";

var ARMATURES = "armatures";
var ARMATURE = "armature";
var BONE = "b";

var ANIMATIONS = "animations";
var ANIMATION = "animation";
var MOVEMENT = "mov";
var EVENT = "event";
var FRAME = "f";

var TEXTURE_ATLAS = "TextureAtlas";
var SUB_TEXTURE = "SubTexture";

var AT = "@";
var A_BONE_TYPE = "bT";
var A_NAME = "name";
var A_START = "st";
var A_DURATION = "dr";
var A_DURATION_TO = "to";
var A_DURATION_TWEEN = "drTW";
var A_LOOP = "lp";
var A_MOVEMENT_SCALE = "sc";
var A_MOVEMENT_DELAY = "dl";

var A_PARENT = "parent";
var A_X = "x";
var A_Y = "y";
var A_SCALE_X = "cX";
var A_SCALE_Y = "cY";
var A_SKEW_X = "kX";
var A_SKEW_Y = "kY";
var A_Z = "z";
var A_HIDE = "hide";
var A_IMAGE = "img";
var A_EVENT = "evt";
var A_SOUND = "sd";
var A_SOUND_EFFECT = "sdE";
var A_TWEEN_EASING ="twE";
var A_TWEEN_ROTATE ="twR";
var A_IS_ARMATURE = "isArmature";
var A_MOVEMENT = "mov";

var A_WIDTH = "width";
var A_HEIGHT = "height";
var A_PIVOT_X = "pX";
var A_PIVOT_Y = "pY";

var A_BONE_IMAGE = "bi";
var A_REGISTATION_X = "regX";
var A_REGISTATION_Y = "regY";

var V_SOUND_LEFT = "l";
var V_SOUND_RIGHT = "r";
var V_SOUND_LEFT_TO_RIGHT = "lr";
var V_SOUND_RIGHT_TO_LEFT = "rl";
var V_SOUND_FADE_IN = "in";
var V_SOUND_FADE_OUT = "out";

var MOVIE_CLIP = "movie clip";
var GRAPHIC = "graphic";
var STRING = "string";
var LABEL_TYPE_NAME = "name";
var DELIM_CHAR = "|";
var EVENT_PREFIX = "@";
var MOVEMENT_PREFIX = "#";
var NO_EASING = "^";

var SKELETON_PANEL = "SkeletonSWFPanel";
var ARMATURE_DATA = "armatureData";
var ANIMATION_DATA = "animationData";

var TEXTURE_SWF_ITEM = "textureSWFItem";
var TEXTURE_SWF = "armatureTextureSWF.swf";

var ALERT = "[ALERT]";
var INFO = "[INFO]";

var pointTemp = {x:0, y:0, skewX:0, skewY:0};

function getArmatureList(_items){
	fl.outputPanel.clear();
	dom.exitEditMode();
	var _arr = [];
	for each(var _item in _items){
		if((_item.symbolType == MOVIE_CLIP || _item.symbolType == GRAPHIC) && _item.name != TEXTURE_SWF_ITEM && isArmatureItem(_item)){
			_arr.push(_item.name);
		}
	}
	return _arr;
}

function generateArmature(_itemName){

	trace(_itemName);
	var _item = library.items[library.findItemIndex(_itemName)];
	var _layersFiltered = isArmatureItem(_item);
	if(!_layersFiltered){
		return;
	}
	var _domName = dom.name.split(".")[0];
	
	var _xml = <{SKELETON} {A_NAME} = {_domName}/>;
	var _armaturesXML = <{ARMATURES}/>;
	var _animationsXML = <{ANIMATIONS}/>;
	var _textureAtlasXML = <{TEXTURE_ATLAS} {A_NAME} = {_domName}/>;
	_xml.appendChild(_armaturesXML);
	_xml.appendChild(_animationsXML);
	_xml.appendChild(_textureAtlasXML);
	
	//骨架名，并备份到 data，增强同步稳定
	var _armatureName = formatName(_item);
	
	var _armatureXML = <{ARMATURE} {A_NAME} = {_armatureName}/>;
	var _animationXML = <{ANIMATION} {A_NAME} = {_armatureName}/>;
	
	var _connection = getArmatureConnection(_item);
	var _armatureConnectionXML = _connection?XML(_connection):_armatureXML.copy();
	
	_armaturesXML.appendChild(_armatureXML);
	var _mainLayer = _layersFiltered.shift();
	//只有1个 movement 且movement.duration只有1，则定义没有动画的骨骼
	if(_mainLayer.frameCount > 1){
		_animationsXML.appendChild(_animationXML);
	}else{
		var _noAnimation = true;
	}
	
	var _keyFrames = filterKeyFrames(_mainLayer.frames);
	var _length = _keyFrames.length;
	var _nameDic = {};
	var _frame;
	var _mainFrame;
	var _isEndFrame;
	
	for(var _iF = 0;_iF < _length;_iF ++){
		_frame = _keyFrames[_iF];
		if(isMainFrame(_frame)){
			//新帧
			_mainFrame = {};
			_mainFrame.frame = _frame;
			_mainFrame.duration = _frame.duration;
			formatSameName(_frame, _nameDic);
		}else if(_mainFrame){
			//继续
			_mainFrame.duration += _frame.duration;
			if(_iF + 1 != _length){
				_mainFrame[_frame.startFrame] = _frame;
			}
		}else{
			//忽略
			continue;
		}
		_isEndFrame = _iF + 1 == _length || isMainFrame(_keyFrames[_iF + 1]);
		if(_mainFrame && _isEndFrame){
			//结束前帧
			//checkTime(_armatureName);
			
			generateMovement(_item, _mainFrame, _layersFiltered, _noAnimation, _armatureXML, _animationXML, _textureAtlasXML, _armatureConnectionXML);
		}
		
	}
	
	setArmatureConnection(_item, _armatureXML.toXMLString());
	return _xml;
}

function generateMovement(_item, _mainFrame, _layers, _noAnimation, _armatureXML, _animationXML, _textureAtlasXML, _armatureConnectionXML){
	var _start = _mainFrame.frame.startFrame;
	var _duration = _mainFrame.duration;
	var _movementXML = createMovementXML(_item, _mainFrame.frame.name, _duration);
			
	var _symbol;
	var _boneName;
	var _boneType;
	var _movementBoneXML;
	var _frameXML;
	var _str;
	
	var _frameStart;
	var _frameDuration;
	
	var _boneNameDic = {};
	var _boneZDic = {};
	var _zList = [];
	var _boneList;
	var _z;
	var _i;
	
	for each(var _layer in _layers){
		for each(var _frame in filterKeyFrames(_layer.frames.slice(_start, _start + _duration))){
			_symbol = getBoneSymbol(_frame.elements);
			if(!_symbol){
				continue;
			}
			
			if(_frame.startFrame < _start){
				_frameStart = 0;
				_frameDuration = _frame.duration - _start + _frame.startFrame;
			}else if(_frame.startFrame + _frame.duration > _start + _duration){
				_frameStart = _frame.startFrame - _start;
				_frameDuration = _duration - _frame.startFrame + _start;
			}else{
				_frameStart = _frame.startFrame - _start;
				_frameDuration= _frame.duration;
			}
			
			/*switch(_frame.tweenType){
				case "motion":
					break;
				case "motion object":
					break;
			}*/
			
			if(_symbol.name){
				//按照实例名索引的骨骼
				_boneType = 1;
				_boneName = formatName(_symbol);
			}else{
				//按照图层名索引的骨骼
				_boneType = 0;
				_boneName = formatName(_layer);
			}
			
			for(_i = _frameStart ;_i < _frameStart + _frameDuration;_i ++){
				_z = _zList[_i];
				if(isNaN(_z)){
					_zList[_i] = _z = 0;
				}else{
					_zList[_i] = ++_z;
				}
			}
			if(!_boneZDic[_boneName]){
				_boneZDic[_boneName] = [];
			}
			_boneList = _boneZDic[_boneName];
			for(_i = _frameStart;_i < _frameStart + _frameDuration;_i ++){
				if(isNaN(_boneList[_i])){
				}else if(_boneType == 1){
					_boneName = formatSameName(_symbol, _boneNameDic);
					if(!_boneZDic[_boneName]){
						_boneList = _boneZDic[_boneName] = [];
					}
				}else if(_boneType == 0){
					_boneName = formatSameName(_layer, _boneNameDic);
					if(!_boneZDic[_boneName]){
						_boneList = _boneZDic[_boneName] = [];
					}
				}
				_boneList[_i] = _z;
			}
			
			_movementBoneXML = createMovementBone(_item, _movementXML, _boneName);
			_frameXML = generateFrame(_layers, Math.max(_frame.startFrame, _start), _z, _symbol, _boneName, _boneType, _noAnimation, _armatureXML, _armatureConnectionXML);
			
			//补间
			if(isNoEasingFrame(_frame)){
				//带有"^"标签的关键帧，将不会被补间
				_frameXML[AT + A_TWEEN_EASING] = NaN;
			}else if(_frame.tweenType == "motion"){
				_frameXML[AT + A_TWEEN_EASING] = formatNumber(_frame.tweenEasing * 0.01);
				var _tweenRotate;
				switch(_frame.motionTweenRotate){
					case "clockwise":
						_tweenRotate = _frame.motionTweenRotateTimes;
						break;
					case "counter-clockwise":
						_tweenRotate = - _frame.motionTweenRotateTimes;
						break;
				}
				//tweenRotate属性应留给补间的到点而不是起点，需要等_movementBoneXML建立完毕后向下传递
				if(_tweenRotate){
					_frameXML[AT + A_TWEEN_ROTATE] = _tweenRotate;
				}
			}
			
			_str = isSpecialFrame(_frame, MOVEMENT_PREFIX, true);
			if(_str){
				_frameXML[AT + A_MOVEMENT] = _str;
			}
			_frameXML[AT + A_START] = _frameStart;
			_frameXML[AT + A_DURATION] = _frameDuration;
			_movementBoneXML.appendChild(_frameXML);
			
			//event
			_str = isSpecialFrame(_frame, EVENT_PREFIX, true);
			if(_str){
				_frameXML[AT + A_EVENT] = _str;
			}

			//sound
			if(_frame.soundName){
				_frameXML[AT + A_SOUND] = _frame.soundLibraryItem.linkageClassName || _frame.soundName;
				var _soundEffect;
				switch(_frame.soundEffect){
					case "left channel":
						_soundEffect = V_SOUND_LEFT;
						break;
					case "right channel":
						_soundEffect = V_SOUND_RIGHT;
						break;
					case "fade left to right":
						_soundEffect = V_SOUND_LEFT_TO_RIGHT;
						break;
					case "fade right to left":
						_soundEffect = V_SOUND_RIGHT_TO_LEFT;
						break;
					case "fade in":
						_soundEffect = V_SOUND_FADE_IN;
						break;
					case "fade out":
						_soundEffect = V_SOUND_FADE_OUT;
						break;
				}
				if(_soundEffect){
					_frameXML[AT + A_SOUND_EFFECT] = _soundEffect;
				}
			}
		}
	}
	
	var _prevFrameXML;
	var _prevStart;
	var _prevDuration;
	var _imageName;
	
	for each(var _movementBoneXML in _movementXML[BONE]){
		_imageName = null;
		for each(_frameXML in _movementBoneXML[FRAME]){
			if(_frameXML[AT + A_IMAGE] == _imageName){
				delete _frameXML[AT + A_IMAGE];
			}else{
				_imageName = _frameXML[AT + A_IMAGE];
				if(Number(_frameXML[AT + A_IS_ARMATURE]) != 1){
					createTextureXML(_textureAtlasXML, _imageName);
					
				}
			}
			
			_frameStart = Number(_frameXML[AT + A_START]);
			if(_frameXML.childIndex() == 0){
				if(_frameStart > 0){
					_movementBoneXML.prependChild(<{FRAME} {A_DURATION} = {_frameStart} {A_HIDE} = "1"/>);
				}
			}else {
				_prevStart = Number(_prevFrameXML[AT + A_START]);
				_prevDuration = Number(_prevFrameXML[AT + A_DURATION]);
				if(_frameStart > _prevStart + _prevDuration){
					_movementBoneXML.insertChildBefore(_frameXML, <{FRAME} {A_DURATION} = {_frameStart - _prevStart - _prevDuration} {A_HIDE} = "1"/>);
				}
			}
			if(_frameXML.childIndex() == _movementBoneXML[FRAME].length() - 1){
				_frameStart = Number(_frameXML[AT + A_START]);
				_prevDuration = Number(_frameXML[AT + A_DURATION]);
				if(_frameStart + _prevDuration < _duration){
					_movementBoneXML.appendChild(<{FRAME} {A_DURATION} = {_duration - _frameStart - _prevDuration} {A_HIDE} = "1"/>);
				}
			}
			_prevFrameXML = _frameXML;
		}
	}
	delete _movementXML[BONE][FRAME][AT + A_START];
	_animationXML.appendChild(_movementXML);
}

function generateFrame(_layers, _start, _z, _bone, _boneName, _boneType, _noAnimation, _armatureXML, _armatureConnectionXML){
	var _frameXML = <{FRAME}/>;
	//寻找骨骼配置，读取父骨骼关系
	var _boneXML = _armatureXML[BONE].(@name == _boneName)[0];
	var _parentName;
	var _parent;
	if(_boneXML){
		_parentName = _boneXML[AT + A_PARENT][0];
	}else{
		//没有骨骼配置，则寻找内置数据
		_boneConnectionXML = _armatureConnectionXML[BONE].(@name == _boneName)[0];
		if(_boneConnectionXML){
			_parentName = _boneConnectionXML[AT + A_PARENT][0];
		}
	}
	//查找父骨骼
	if(_parentName){
		_parent = getBoneFromLayers(_layers, _parentName, _start);
	}
	if (_parent) {
		transfromParentPoint(pointTemp, _bone, _parent);
		pointTemp.skewX = _bone.skewX - _parent.skewX;
		pointTemp.skewY = _bone.skewY - _parent.skewY;
	}else {
		pointTemp.x = _bone.x;
		pointTemp.y = _bone.y;
		pointTemp.skewX = _bone.skewX;
		pointTemp.skewY = _bone.skewY;
	}
	
	var _imageItem = _bone.libraryItem;
	var _imageName = formatName(_imageItem).replace("/", "-");

	if(!_boneXML){
		//没有骨骼配置，则根据当前骨骼创建

		_boneXML = createBoneXML(_boneName, _parentName, pointTemp, _boneType, _z, _imageName);
		_armatureXML.appendChild(_boneXML);
	}
	if(!_parent){
		//未找到父骨骼则删除错误标签
		//不应出现骨骼扔拥有子骨骼的时候，却在时间轴上删除该骨骼
		delete _boneXML[AT + A_PARENT];
	}
	//x、y、skewX、skewY为相对数据
	_frameXML[AT + A_X] = formatNumber(pointTemp.x - Number(_boneXML[AT + A_X]));
	_frameXML[AT + A_Y] = formatNumber(pointTemp.y - Number(_boneXML[AT + A_Y]));
	_frameXML[AT + A_SKEW_X] = formatNumber(pointTemp.skewX - Number(_boneXML[AT + A_SKEW_X]));
	_frameXML[AT + A_SKEW_Y] = formatNumber(pointTemp.skewY - Number(_boneXML[AT + A_SKEW_Y]));
	_frameXML[AT + A_SCALE_X] = formatNumber(_bone.scaleX);
	_frameXML[AT + A_SCALE_Y] = formatNumber(_bone.scaleY);
	_frameXML[AT + A_Z] = _z;
	
	
	if(_imageItem.symbolType != MOVIE_CLIP){
		_imageItem.symbolType = MOVIE_CLIP;
	}
	_frameXML[AT + A_IMAGE] = _imageName;
	var _isArmature = isArmatureItem(_imageItem);
	if(_isArmature){
		_frameXML[AT + A_IS_ARMATURE] = 1;
	}
	if(_noAnimation){
		if(!_boneXML[AT + A_IMAGE][0]){
			_boneXML[AT + A_IMAGE] = _imageName;
			if(_isArmature){
				_boneXML[AT + A_IS_ARMATURE] = 1;
			}
		}
	}
	return _frameXML;
}

function createMovementXML(_armature, _movementName, _duration){
	if(_armature.hasData(ANIMATION_DATA)){
		var _animationXML = XML(_armature.getData(ANIMATION_DATA));
		var _xml = _animationXML[MOVEMENT].(@name == _movementName)[0];
	}
	if(!_xml){
		_xml = <{MOVEMENT} {A_NAME} = {_movementName}/>;
		_xml[AT + A_DURATION_TO] = 6;
	}
	_xml[AT + A_DURATION] = _duration;
	if(_duration > 1){
		if(!_xml[AT + A_DURATION_TWEEN][0]){
			_xml[AT + A_DURATION_TWEEN] = _duration > 2?_duration:10;
		}
		if(_duration == 2){
			if(!_xml[AT + A_LOOP][0]){
				_xml[AT + A_LOOP] = 1;
			}
			if(!_xml[AT + A_TWEEN_EASING][0]){
				_xml[AT + A_TWEEN_EASING] = 2;
			}
		}
	}
	return _xml;
}

function createMovementBone(_armature, _movementXML, _boneName){
	var _xml = _movementXML[BONE].(@name == _boneName)[0];
	if(!_xml){
		_xml = <{BONE} {A_NAME} = {_boneName}/>;
		_xml[AT + A_MOVEMENT_SCALE] = 1;
		_xml[AT + A_MOVEMENT_DELAY] = 0;
		_movementXML.appendChild(_xml);
	}
	return _xml;
}

function createBoneXML(_name, _parentName, _point, _boneType, _z, _imageName){
	var _xml = <{BONE} {A_NAME} = {_name} {A_BONE_TYPE} = {_boneType} {A_BONE_IMAGE} = {_imageName.replace("/","-")}/>;
	if(_parentName){
		_xml[AT + A_PARENT] = _parentName;
	}
	_xml[AT + A_X] = formatNumber(_point.x);
	_xml[AT + A_Y] = formatNumber(_point.y);
	_xml[AT + A_SKEW_X] = formatNumber(_point.skewX);
	_xml[AT + A_SKEW_Y] = formatNumber(_point.skewY);
	_xml[AT + A_Z] = _z;
	return _xml;
}

//记录贴图
function createTextureXML(_textureAtlasXML, _imageName){
	var _xml = _textureAtlasXML[SUB_TEXTURE].(@name == _imageName)[0];
	if(!_xml){
		_xml = <{SUB_TEXTURE} {A_NAME} = {_imageName}/>;
		_textureAtlasXML.appendChild(_xml);
	}
	return _xml;
}

//通过骨架名写入骨架关联数据
function changeArmatureConnection(_armatureName, _data){
	if(errorDOM()){
		return "";
	}
	var _armature = library.items[library.findItemIndex(_armatureName)];
	if(!_armature){
		trace(ALERT, "未找到 " + _armatureName + " 元件，请确认保持 FLA 文件同步！");
		return "";
	}
	_data = XML(_data).toXMLString();
	_data = replaceString(_data, "&lt;", "<");
	_data = replaceString(_data, "&gt;", ">");
	setArmatureConnection(_armature, _data);
}

function changeMovement(_armatureName, _movementName, _data){
	if(errorDOM()){
		return "";
	}
	var _armature = library.items[library.findItemIndex(_armatureName)];
	if(!_armature){
		trace(ALERT, "未找到 " + _armatureName + " 元件，请确认保持 FLA 文件同步！");
		return "";
	}
	
	_data = XML(_data).toXMLString();
	_data = replaceString(_data, "&lt;", "<");
	_data = replaceString(_data, "&gt;", ">");
	_data = XML(_data);
	
	var _animationXML;
	if(_armature.hasData(ANIMATION_DATA)){
		_animationXML = XML(_armature.getData(ANIMATION_DATA));
	}else{
		_animationXML = <{ANIMATION}/>;
	}
	var _movementXML = _animationXML[MOVEMENT].(@name == _movementName)[0];
	if(_movementXML){
		_animationXML[MOVEMENT][_movementXML.childIndex()] = _data;
	}else{
		_animationXML.appendChild(_data);
	}
	delete _data[BONE].*;
	_armature.addData(ANIMATION_DATA, STRING, _animationXML.toXMLString());
}


//获取骨架关联数据
function getArmatureConnection(_armature){
	if(_armature.hasData(ARMATURE_DATA)){
		return _armature.getData(ARMATURE_DATA);
	}
	return null;
}

//写入骨架关联数据
function setArmatureConnection(_armature, _data){
	_armature.addData(ARMATURE_DATA, STRING, _data);
}

//是否复合 armature 结构，如果是返回 mainLayer 和 boneLayers
function isArmatureItem(_item){
	var _layersFiltered = [];
	var _mainLayer;
	for each(var _layer in _item.timeline.layers){
		switch(_layer.layerType){
			case "folder":
			case "guide":
			case "mask":
				break;
			default:
				if(isMainLayer(_layer)){
					_mainLayer = _layer;
				}else if(!isBlankLayer(_layer)){
					_layersFiltered.unshift(_layer);
				}
				break;
		}
	}
	
	if(_mainLayer && _layersFiltered.length > 0){
		_layersFiltered.unshift(_mainLayer);
		return _layersFiltered;
	}
	return null;
}

//是否为主标签层
function isMainLayer(_layer){
	for each(var _frame in filterKeyFrames(_layer.frames)){
		if(isMainFrame(_frame)){
			return true;
		}
	}
	return false;
}

//是否为主关键帧
function isMainFrame(_frame){
	return _frame.labelType == LABEL_TYPE_NAME && !isNoEasingFrame(_frame) && !isSpecialFrame(_frame, EVENT_PREFIX) && !isSpecialFrame(_frame, MOVEMENT_PREFIX);
}

//是否为不补间关键帧
function isNoEasingFrame(_frame){
	return _frame.labelType == LABEL_TYPE_NAME && _frame.name.indexOf(NO_EASING) >= 0;
}

//是否为事件关键帧
function isSpecialFrame(_frame, _framePrefix, _returnName){
	var _b = _frame.labelType == LABEL_TYPE_NAME && _frame.name.indexOf(_framePrefix) >= 0 && _frame.name.length > 1;
	if(_b && _returnName){
		var _arr = _frame.name.split(DELIM_CHAR);
		for each(var _str in _arr){
			if(_str.indexOf(_framePrefix) == 0){
				return _str.substr(1);
			}
		}
		trace(ALERT, "错误的特殊关键帧命名！", _frame.name);
		return false;
	}
	return _b;
}

//是否为空图层
function isBlankLayer(_layer){
	for each(var _frame in filterKeyFrames(_layer.frames)){
		if(_frame.elements.length){
			return false;
		}
	}
	return true;
}

//获取骨骼
function getBoneFromLayers(layers, _boneName, _frameIndex){
	var _symbol;
	var _layerBones = [];
	for each(var _layer in layers){
		if(_layer.name == _boneName){
			return getBoneSymbol(_layer.frames[_frameIndex].elements);
		}
	}
	return null;
}

//过滤符合骨骼的元素
function getBoneSymbol(_elements){
	var _symbolsNamed = [];
	var _elementUnnamed;
	for each(var _element in _elements){
		if(_element.symbolType == MOVIE_CLIP || _element.symbolType == GRAPHIC){
			return _element;
		}
	}
	return null;
}

//过滤关键帧
function filterKeyFrames(_frames){
	var _framesCopy = [];
	for each(var _frame in _frames){
		if(_framesCopy.indexOf(_frame)>=0){
			continue;
		}
		_framesCopy.push(_frame);
	}
	return _framesCopy;
}

//转换父坐标系
function transfromParentPoint(_point, _bone, _parent){
	var _dX = _bone.x - _parent.x;
	var _dY = _bone.y - _parent.y;
	var _r = Math.atan2(_dY, _dX) - _parent.skewY * Math.PI / 180;
	var _len = Math.sqrt(_dX * _dX + _dY * _dY);
	_point.x = _len * Math.cos(_r);
	_point.y = _len * Math.sin(_r);
}

//避开同名
function formatSameName(_obj, _dic){
	var _i = 0;
	var _name = formatName(_obj);
	while(_dic?_dic[_name]:_i == 0){
		_name = _obj.name + _i;
		_i ++;
	}
	if(_i > 0){
		_obj.name = _name;
	}
	if(_dic){
		_dic[_name] = true;
	}
	return _name;
}

//防止对象未命名
function formatName(_obj){
	var _name = _obj.name;
	if(!_name){
		_obj.name = _name = "unnamed" + Math.round((Math.random()*10000));
	}else if(_name.indexOf(DELIM_CHAR) >= 0){
		_obj.name = _name = replaceString(_name, DELIM_CHAR, "");
	}
	return _name;
}

//保留小数
function formatNumber(_num, _retain){
	_retain = _retain || 100;
	return Math.round(_num * _retain) / 100;
}

function replaceString(_strOld, _str, _rep){
	if(_strOld){
		return _strOld.split(_str).join(_rep);
	}
	return "";
}

function clearTextureSWFItem(_length){
	if(!library.itemExists(TEXTURE_SWF_ITEM)){
		library.addNewItem(MOVIE_CLIP, TEXTURE_SWF_ITEM);
	}
	library.editItem(TEXTURE_SWF_ITEM);
	var _timeline = dom.getTimeline();
	_timeline.currentLayer = 0;
	_timeline.removeFrames(0, _timeline.frameCount);
	_timeline.insertBlankKeyframe(0);
	_timeline.insertFrames(0, _length + 2);
	_timeline.convertToKeyframes(0, _length + 2);
	_timeline.layers[0].frames[0].actionScript = "stop();";
}

function addTextureToSWFItem(_itemName,  _instanceName, _index){
	var _timeline = dom.getTimeline();
	_timeline.currentFrame = 0;
	library.addItemToDocument({x:0, y:0}, _itemName.replace("-", "/"));
	_timeline.currentFrame = _index;
	library.addItemToDocument({x:0, y:0}, _itemName.replace("-", "/"));
	_timeline.layers[0].frames[_index].name = _itemName;
	_timeline.layers[0].frames[_index].elements[0].name = _itemName;
}

function packTexturesCustom(_xmlOrBoolean){
	trace("自定义排序");
}

function packTextures(_widthMax, _interval, _verticalSide, _textureAtlasXML){
	if(errorDOM()){
		return;
	}
	if(!library.itemExists(TEXTURE_SWF_ITEM)){
		return;
	}
	if(_textureAtlasXML){
		_textureAtlasXML = XML(_textureAtlasXML).toXMLString();
		_textureAtlasXML = replaceString(_textureAtlasXML, "&lt;", "<");
		_textureAtlasXML = replaceString(_textureAtlasXML, "&gt;", ">");
		_textureAtlasXML = XML(_textureAtlasXML);
	}
	
	var _textureItem = library.items[library.findItemIndex(TEXTURE_SWF_ITEM)];
	var _timeline = dom.getTimeline();
	_timeline.currentFrame = 0;
	var _textureList = [];
	var _name;
	//var _item;
	for each(var _texture in _textureItem.timeline.layers[0].frames[0].elements){
		if(_texture.symbolType == MOVIE_CLIP || _texture.symbolType == GRAPHIC){
			//_item = _texture.libraryItem;
			if(_texture.scaleX != 1){
				_texture.scaleX = 1;
			}
			if(_texture.scaleY != 1){
				_texture.scaleY = 1;
			}
			if(_texture.skewX != 0){
				_texture.skewX = 0;
			}
			if(_texture.skewY != 0){
				_texture.skewY = 0;
			}
			_textureList.push(_texture);
		}
	}
	
	if (_textureList.length == 0) {
		return;
	}
	
	_interval = _interval || 0;
	//贴图按照大小排序
	_textureList.sort(sortTextureList);
	
	_widthMax = getNearest2N(Math.max(_textureList[0].width, _widthMax));
	
	//预置一个较高的高度
	var _heightMax = 40960;
	
	var _remainRectList = [createRectangle(0, 0, _widthMax, _heightMax)];
	
	var _isFit;
	var _width;
	var _height;
	var _pivotX;
	var _pivotY;
	
	var _rect;
	var _rectID;
	var _rectPrev;
	var _rectNext;
	var _step = 0;
	var _subTexture;
	do {
		//寻找最高的空白区域
		_rect = getHighestRect(_remainRectList);
		_rectID = _remainRectList.indexOf(_rect);
		_isFit = false;
		for(var _iT in _textureList) {
			//逐个比较贴图对象是否适合该区域
			_texture = _textureList[_iT];
			_width = Math.ceil(_texture.width) + _interval;
			_height = Math.ceil(_texture.height) + _interval;
			if (_rect.width >= _width && _rect.height >= _height) {
				//考虑竖直贴图的合理摆放
				if (_verticalSide?(_texture.height > _texture.width * 4?(_rectID > 0?(_rect.height - _height >= _remainRectList[_rectID - 1].height):true):true):true){
					_isFit = true;
					break;
				}
			}
		}
		if(_isFit){
			//如果合适，放置贴图，并将矩形区域再次分区
			_pivotX = _texture.x - _texture.left;
			_pivotY = _texture.y - _texture.top;
			_texture.x = _rect.x + _pivotX;
			_texture.y = _rect.y + _pivotY;
			if(_textureAtlasXML){
				_subTexture = createTextureXML(_textureAtlasXML, _texture.libraryItem.name.replace("/","-"));
				_subTexture[AT + A_X] = _rect.x;
				_subTexture[AT + A_Y] = _rect.y;
				_subTexture[AT + A_WIDTH] = _width - _interval + 1;
				_subTexture[AT + A_HEIGHT] = _height - _interval + 1;
				_subTexture[AT + A_PIVOT_X] = formatNumber(_pivotX);
				_subTexture[AT + A_PIVOT_Y] = formatNumber(_pivotY);
			}
			_textureList.splice(Number(_iT), 1);
			_remainRectList.splice(_rectID + 1, 0, createRectangle(_rect.x + _width, _rect.y, _rect.width - _width, _rect.height));
			_rect.y += _height;
			_rect.width = _width;
			_rect.height -= _height;
		}else{
			//不合适，则放弃这个矩形区域，把这个区域将与他相邻的矩形区域合并（与较高的一边合并）
			if(_rectID == 0){
				_rectNext = _remainRectList[_rectID + 1];
			}else if(_rectID == _remainRectList.length - 1){
				_rectNext = _remainRectList[_rectID - 1];
			}else{
				_rectPrev = _remainRectList[_rectID - 1];
				_rectNext = _remainRectList[_rectID + 1];
				_rectNext = _rectPrev.height <= _rectNext.height?_rectNext:_rectPrev;
			}
			if(_rect.x < _rectNext.x){
				_rectNext.x = _rect.x;
			}
			_rectNext.width = _rect.width + _rectNext.width;
			_remainRectList.splice(_rectID, 1);
		}
		_step ++;
	}while (_textureList.length > 0);
	if(_textureAtlasXML){
		_heightMax = getNearest2N(_heightMax - getLowestRect(_remainRectList).height);
		_textureAtlasXML[AT + A_WIDTH] = _widthMax;
		_textureAtlasXML[AT + A_HEIGHT] = _heightMax;
	}
	dom.selectAll();
	dom.selectNone();
	return _textureAtlasXML;
}

function exportSWF(){
	if(!library.itemExists(TEXTURE_SWF_ITEM)){
		return;
	}
	var _folderURL = fl.configURI;
	var _pathDelimiter;
	if(_folderURL.indexOf("/")>=0){
		_pathDelimiter = "/";
	}else if(_folderURL.indexOf("\\")>=0){
		_pathDelimiter = "\\";
	}else{
		return "";
	}
	
	_folderURL = _folderURL + "WindowSWF" + _pathDelimiter + SKELETON_PANEL;
	
	if(!FLfile.exists(_folderURL)){
		FLfile.createFolder(_folderURL);
	}
	
	var _swfURL = _folderURL + _pathDelimiter + TEXTURE_SWF;
	library.items[library.findItemIndex(TEXTURE_SWF_ITEM)].exportSWF(_swfURL);
	return _swfURL;
}
		
function sortTextureList(_instance1, _instance2){
	var _v1=_instance1.width + _instance1.height;
	var _v2=_instance2.width + _instance2.height;
	if (_v1 == _v2) {
		return _instance1.width > _instance2.width?-1:1;
	}
	return _v1 > _v2?-1:1;
}

function getNearest2N(_n){
	return _n & _n - 1?1 << _n.toString(2).length:_n;
}

function createRectangle(_x, _y, _width, _height){
	return {x:_x||0, y:_y||0, width:_width||0, height:_height||0};
}

function getInstanceRect(_instance){
	return createRectangle(_instance.x - _instance.left, _instance.y - _instance.top, _instance.width, _instance.height);
}

function getHighestRect(_rectList){
	var _height = 0;
	var _rectHighest;
	for each(var _rect in _rectList) {
		if (_rect.height > _height) {
			_height = _rect.height;
			_rectHighest = _rect;
		}
	}
	return _rectHighest;
}
	
function getLowestRect(_rectList){
	var _height = 40960;
	var _rectLowest;
	for each(var _rect in _rectList) {
		if (_rect.height < _height) {
			_height = _rect.height;
			_rectLowest = _rect;
		}
	}
	return _rectLowest;
}