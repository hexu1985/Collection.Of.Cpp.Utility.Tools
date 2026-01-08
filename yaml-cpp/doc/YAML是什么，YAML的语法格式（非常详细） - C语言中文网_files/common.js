// jQuery cookie 插件
(function($,document,undefined){var pluses=/\+/g;function raw(s){return s}function decoded(s){return unRfc2068(decodeURIComponent(s.replace(pluses," ")))}function unRfc2068(value){if(value.indexOf('"')===0){value=value.slice(1,-1).replace('\\"','"').replace("\\\\","\\")}return value}var config=$.cookie=function(key,value,options){if(value!==undefined){options=$.extend({},config.defaults,options);if(value===null){options.expires=-1}if(typeof options.expires==="number"){var days=options.expires,t=options.expires=new Date();t.setDate(t.getDate()+days)}value=config.json?JSON.stringify(value):String(value);return(document.cookie=[encodeURIComponent(key),"=",config.raw?value:encodeURIComponent(value),options.expires?"; expires="+options.expires.toUTCString():"",options.path?"; path="+options.path:"",options.domain?"; domain="+options.domain:"",options.secure?"; secure":""].join(""))}var decode=config.raw?raw:decoded;var cookies=document.cookie.split("; ");var result=key?null:{};for(var i=0,l=cookies.length;i<l;i++){var parts=cookies[i].split("=");var name=decode(parts.shift());var cookie=decode(parts.join("="));if(config.json){cookie=JSON.parse(cookie)}if(key&&key===name){result=cookie;break}if(!key){result[name]=cookie}}return result};config.defaults={};$.removeCookie=function(key,options){if($.cookie(key)!==null){$.cookie(key,null,options);return true}return false}})(jQuery,document);

window.config = {
	'siteid': 2,
	'serverDomain': 'https://vip.biancheng.net',
	'rootDomain'  : 'biancheng.net'
};

$(document).ready(function(){
	//上一节/下一节
	(function(){
		$("#article").children(".pre-next-page").html(
			'<span class="pre left"><a href="' + window.prePageURL + '"><span class="icon">&lt;</span> 上一节</a></span>' +
			'<span class="next right"><a href="' + window.nextPageURL + '">下一节 <span class="icon">&gt;</span></a></span>'
		);
	})();

	// 在目录中找到当前文章
	(function(){
		var url = document.location.pathname;
		var contents = $("#contents");
		var as = contents.length ? contents.find('a') : null;

		as && as.each(function(index, domEle){
			var thisURL = $(domEle).attr('href');
			if(thisURL === url){
				$(this).parent().addClass('active');
				return false;
			}
		});
	})();

	//手机版--教程目录切换按钮
	(function(){
		var toggleBtn = $("#sidebar-toggle");
		var sidebar = $("#sidebar");
		toggleBtn.click(function(){
			toggleBtn.toggleClass("toggle-btn-active");
			sidebar.toggleClass("toggle-target-active");
		});
	})();

	//返回顶部
	(function(){
		$("#return-top").click(function(){
			$(window).scrollTop(0);
		});
	})();


	/*//Trae IDE
	(function(){
		var tempProtocol = 'https';
		var tempDomainP1 = 'www';
		var tempDomainP2 = 'trae';
		var tempDomainP3 = 'com.cn';
		var tempPath = '/';
		var tempParams1 = 'utm_medium=zhong' + 'wenwang_ug_cpa';
		var tempParams2 = 'utm_source=a' + 'dvertising';
		var tempParams3 = 'utm_term=hw_trae_zhongwenwang';
		var materialName = 'trae-ide.png';
		$("#trae-ide").html(
			'<a href="' + tempProtocol + '://' + tempDomainP1 + '.' + tempDomainP2 + '.' + tempDomainP3 + tempPath + '?' + tempParams1 + '&' + tempParams2 + '&' + tempParams3 + '" target="_blank">' +
				'<img width="100%" src="/uploads/material/' + materialName + '?v=' + window.cmsTempletsVer + '" alt="trae-ide">' +
			'</a>'
		);
	})();*/

	// 加载代码高亮插件
	function codeHighlight(){
		var pres = document.getElementsByTagName("pre");
		if(!pres || !pres.length) return;

		jQuery.ajaxSetup({
			cache: true
		});
		$.getScript(window.cmsTemplets+"/script/jquery.snippet.js",function(){
			$(pres).each(function(){
				settings = {style: "bright"};

				var thisClass = $(this).attr("class");
				thisClass = thisClass && thisClass.replace( /^shell$/, "sh" );  // Shell

				var showNum = $(this).attr("shownum");
				if(showNum && showNum=="false"){
					settings.showNum = false;
					settings.menu = false;
				}

				var showMenu = $(this).attr("showmenu");
				if(showMenu){
					if(showMenu=="false"){
						settings.menu = false;
					}else{
						settings.menu = true;
					}
				}

				if(showMenu && showMenu=="false"){
					settings.menu = false;
				}

				thisClass && !/info-box/.test(thisClass) && $(this).snippet(thisClass, settings);
			});
		});
		jQuery.ajaxSetup({
			cache: false
		});
	}
	codeHighlight();

	//jsonp加载文章
	(function(){
		var buyCourseTip = $("#buy-course-tip");
		if(buyCourseTip && buyCourseTip.length){
			var vipCookie = parseInt( $.cookie("cpvip") );
			var pointsCookie = parseInt( $.cookie("cppoints") );

			if(vipCookie>=7){  //开通了会员
				$.ajax({
					method: 'get',
					url: window.config.serverDomain + "/p/vip/api/ajax/get_article.php",
					dataType: 'jsonp',
					data: {
						'siteid': window.config.siteid,
						'aid':	buyCourseTip.attr("aid"),
						'v':	  window.cmsTempletsVer
					},
					timeout: 10000,
					beforeSend: function(XMLHttpRequest){
						buyCourseTip.html('<p>正在加载文章内容...</p>');
					},
					success: function(retData){
						var arcBody = decodeURIComponent(retData.body);
						if(retData.success){
							buyCourseTip.parent().html( decodeURIComponent(retData.body) );
							codeHighlight();
						}else{
							buyCourseTip.html('<p>' +decodeURIComponent(retData.msg) + '</p>');
						}
					},
					error: function(jqXHR, textStatus, errorThrown){
						buyCourseTip.html('<p>加载文章内容失败，请刷新重试！</p>');
					}
				});
			}else if(pointsCookie>0){  //拥有积分
				var points =  buyCourseTip.attr("points");
				$.ajax({
					method: 'get',
					url: window.config.serverDomain + "/p/points/api/ajax/get_article.php",
					dataType: 'jsonp',
					data: {
						'siteid': window.config.siteid,
						'aid':	buyCourseTip.attr("aid"),
						'points': points ? points : 1,
						'v':	  window.cmsTempletsVer
					},
					timeout: 10000,
					beforeSend: function(XMLHttpRequest){
						buyCourseTip.html('<p>正在加载文章内容...</p>');
					},
					success: function(retData){
						var arcBody = decodeURIComponent(retData.body);
						if(retData.success){
							buyCourseTip.parent().html( decodeURIComponent(retData.body) );
							codeHighlight();
						}else{
							buyCourseTip.html('<p>' +decodeURIComponent(retData.msg) + '</p>');
						}
					},
					error: function(jqXHR, textStatus, errorThrown){
						buyCourseTip.html('<p>加载文章内容失败，请刷新重试！</p>');
					}
				});
			}else if( !$.cookie("cuserinfo") ){
				buyCourseTip.html('<p>您好，您正在阅读高级教程/项目实践，请先 <a href="' + window.config.serverDomain + '/?from=viparc" target="_blank">登录 / 注册</a>。</p>');
			}else{
				buyCourseTip.html('<p>您好，您正在阅读付费教程，请<a href="' + window.config.serverDomain + '/p/vip/show.php?from=viparc" target="_blank">开通VIP会员（赠送交流群）</a>。</p>');
			}
		}
	})();

	//更新文章点击次数
	(function(){
		$.ajax({
			method: 'get',
			url: window.config.serverDomain + "/click/api/click.php",
			dataType: 'jsonp',
			data: {"siteid": window.siteId, "arcid": window.arcId, "v": window.cmsTempletsVer},
			timeout: 10000,
			success: function(data){
				if( !data.success ){
					//TODO:
				}
			},
			error: function(jqXHR, textStatus, errorThrown){
				//TODO:
			}
		});
	})();


	//Tab 切换
	$(".tab").each(function(){
		var tab = $(this),
			tabTitle = tab.children(".tab-title-list"),
			tabCont = tab.children(".tab-cont-list");

		tabTitle.delegate(".list-item", "click", function(){
			var thisTitle = tabTitle.children(".this-item"),
				thisCont = tabCont.children(".this-item"),
				hoverTitle = $(this),
				tabNo = hoverTitle.attr("tabno");

			thisTitle.removeClass('this-item');
			thisCont.removeClass('this-item');
			hoverTitle.addClass("this-item");
			tabCont.children("[tabno='" + tabNo + "']").addClass("this-item");
		});
	});
});