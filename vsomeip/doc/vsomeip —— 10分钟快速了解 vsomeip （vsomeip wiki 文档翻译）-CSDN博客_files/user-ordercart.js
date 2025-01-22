"use strict";!function(t,e){function n(t){var e,n=new RegExp("(^| )"+t+"=([^;]*)(;|$)");return(e=document.cookie.match(n))?unescape(e[2]):null}function i(t){for(var e=document.cookie.split(";"),n={},i=0;i<e.length;i++){var r=e[i].split("="),o=r[0];r.shift();var s=r.join("=");t&&o.toString().trim().lastIndexOf("_app")===o.toString().trim().length-"_app".length?n[o.toString().trim().replace("_app","")]=s:t||0!==o.toString().trim().indexOf("c_")||(n[o.toString().trim()]=s)}return n}function r(){var t=arguments.length>0&&void 0!==arguments[0]?arguments[0]:{};return t.product_id=t.product_id||0,t.goods_id=t.goods_id||0,t.num=t.num||1,t.is_ajax=t.is_ajax||1,t.t=t.t||(new Date).getTime(),t.device_id=s(),t.device_type=u(),t.utm_source_ext=c(t.report_ext||{}),t}function o(){return!!t.navigator.userAgent.toLowerCase().match(/(csdn)/i)}function s(){var t=n("X-App-ID")||"";return"CSDN-APP"==t||"CSDN-EDU"==t?n("X-Device-ID")||"":n("uuid_tt_dd")||""}function a(e){var n=new RegExp("(^|&)"+e+"=([^&]*)(&|$)","i"),i=t.location.search.substr(1).match(n);return null!=i?unescape(i[2]):null}function c(t){var e={};return e=i(o()),o()?e.dc_sid=n("dc_sid_app")||"":e.dc_sid=n("dc_sid")||"",JSON.stringify(Object.assign(e,t))}function u(){var t,e=g.isEDUIos(),i=g.isEDUAndroid(),r=g.isAndroid(),o=n("X-App-ID")||"",s=n("X-OS")||"";return g.isWap()||(t=10),g.isMac()&&(t=11),g.isWap()&&(t=9),i&&r&&(t=3),e&&(t=4),"CSDN-APP"==o&&"Android"==s&&(t=1),"CSDN-APP"==o&&"iOS"==s&&(t=2),"CSDN-EDU"==o&&"Android"==s&&(t=3),"CSDN-EDU"==o&&"iOS"==s&&(t=4),"CSDN-APP"==o&&"OHOS"==s&&(t=13),t}function d(){var e=a("api_env"),n=t.location.host,i="https://mall.csdn.net/",r=document.getElementsByTagName("meta").ordercart;!e&&r&&r.content&&r.content.length?n=JSON.parse(r.content).api_env:e&&(n=e);var o=/^beta|test|loc[a-z]*/;return n.indexOf(".blog.csdn.net")>=0?(i="https://mall.csdn.net/",h="prod"):n.match(o)?(i="https://test-mall.csdn.net/",h="test"):n.match(/^pre-|^pre[a-z]*/)&&(h="pre",i="https://pre-mall.csdn.net/"),i}function l(){var t="";return"pre"==h?t="pre-":"test"==h&&(t="test-"),t}function p(t){var e="";for(var n in t)"function"!=typeof t[n]&&(e+=n+"="+t[n]+"&");return"?"+(e=e.substr(0,e.length-1))}function m(){if(t.csdn.loginBox&&t.csdn.loginBox.show)t.csdn.loginBox.show();else{var e="https://passport.csdn.net/";o()?t.location.href=e+"account/login":("test"==h&&(e="https://test-passport.csdn.net/"),t.location.href=e+"account/login?from="+encodeURIComponent(location.href))}}function _(){this.timer=0,this.timershow=0,this.ident=!0,this.pay_code_timeout=3e5,this.get_status_sec=3e3,this.timeout_time=1e4,this.payment_flag=0,this.payment_function=null,this.no_login=0,this.orderNo="",this.cart_url=d(),this.prefix_domain=l(),this.get_status_url=this.cart_url+"mp/mallorder/pay/getStatus",this.order_number="",this.deviceDistinguish=g}var f=null,h="prod",g={ua:t.navigator.userAgent.toLowerCase(),isEDUIos:function(){return/(?:csdnedu-ipad)/.test(this.ua)},isEDUAndroid:function(){return/(?:csdnedu)/.test(this.ua)},isAndroid:function(){return/(?:android)/.test(this.ua)},isWindowsPhone:function(){return/(?:windows phone)/.test(this.ua)},isSymbian:function(){return/(?:symbianos)/.test(this.ua)||this.isWindowsPhone},isFireFox:function(){return/(?:firefox)/.test(this.ua)},isChrome:function(){return/(?:chrome|crios)/.test(this.ua)},isTablet:function(){return/(?:ipad|playbook)/.test(this.ua)||this.isAndroid&&!/(?:mobile)/.test(this.ua)||this.isFireFox&&/(?:tablet)/.test(this.ua)},isPhone:function(){return/(?:iphone)/.test(this.ua)&&!this.isTablet},isPc:function(){return!this.isPhone&&!this.isAndroid&&!this.isSymbian},isWechatdevtools:function(){return/wechatdevtools/.test(this.ua)},isMac:function(){return t.navigator.platform.toLocaleLowerCase().indexOf("mac")>-1},isMicromessenger:function(){return/micromessenger/.test(this.ua)},isWap:function(){return!!this.ua.match(/(phone|pad|pod|iPhone|iPod|ios|iPad|Android|Mobile|BlackBerry|IEMobile|MQQBrowser|JUC|Fennec|wOSBrowser|BrowserNG|WebOS|Symbian|Windows Phone)/i)}};_.prototype={constructor:_,setDefaultDate:r,clearTimer:function(t){void 0!==this.timer&&clearInterval(this.timer),void 0!==this.timershow&&clearInterval(this.timershow)},buyNow:function(e){var n=document.location.protocol,i="";i=this.deviceDistinguish.isWap()?"order.csdn.net/v2/m/submit":"order.csdn.net/v2/order-m/submit",t.location.href=n+"//"+this.prefix_domain+i+p(e)},buyNowMall:function(n){var i=document.location.protocol,o=this,s=n.params&&n.params.utm_source||"",a="";a=this.deviceDistinguish.isWap()?"order.csdn.net/v2/3/submit-m":"order.csdn.net/v2/3/submit",n.params&&n.params.utmSourceExt||(n.params.utmSourceExt={});var c=r();Object.assign(n.params.utmSourceExt,Object.assign(JSON.parse(decodeURIComponent(c.utm_source_ext||"{}")),{device_id:c.device_id||"",device_type:c.device_type||""}));var u=e.ajax({type:"post",url:o.cart_url+"mp/mallorder/order/buyNow",data:JSON.stringify(n.params),dataType:"json",contentType:"application/json",xhrFields:{withCredentials:!0},timeout:o.timeout_time,success:function(e){if(200==e.code){var r=n.extra||{};return r.cartId=e.data.cartId,a=i+"//"+o.prefix_domain+a+p(r),s&&s.length>0&&(a=a+"&utm_source="+s),n.success?n.success({code:200,data:{cartId:e.data.cartId,url:a}}):t.location.href=a,!1}401==e.code?m():n.fail&&n.fail({code:15,errorMessage:"提交订单失败"})},error:function(t){if(401==t.code)return void m();n.fail&&n.fail({status:15,errorMessage:"提交订单失败"})},complete:function(t,e){"timeout"===e&&u.abort()}})},qrPay:function(t){var n,i,o,s=this;t=r(t),"function"==typeof t.success_function&&(n=t.success_function,delete t.success_function),"function"==typeof t.error_function&&(i=t.error_function,delete t.error_function),"function"==typeof t.timeout_function&&(o=t.timeout_function,delete t.timeout_function),"function"==typeof t.payment_function?(s.payment_function=t.payment_function,delete t.payment_function):s.payment_function=null,"function"==typeof t.get_pay_success_callback&&(f=t.get_pay_success_callback,delete t.get_pay_success_callback),s.payment_flag=0,t.resultJSON=1;var a=e.ajax({type:"get",url:s.cart_url+"mp/mallorder/pay/qrPay",data:t,dataType:"json",timeout:s.timeout_time,crossDomain:!0,xhrFields:{withCredentials:!0},success:function(e){if(1===e.status)return n(t.request_type,e),s.orderNo=e.orderNo,s.clearTimer(),s.get_status_url=e.get_status_url,!0!==t.isClearTimer&&(s.timer=setInterval(function(){s.get_pay_status()},s.get_status_sec)),s.timershow=setInterval(function(){o(),s.clearTimer()},s.pay_code_timeout),!1;i(e)},error:function(){i({status:15,errorMessage:"获取二维码失败"})},complete:function(t,e){"timeout"===e&&a.abort()}})},get_pay_status:function(){var t=arguments.length>0&&void 0!==arguments[0]?arguments[0]:{},n=this;"function"==typeof t.payment_function&&(n.payment_function=t.payment_function),"function"==typeof t.get_pay_success_callback&&(f=t.get_pay_success_callback),t.get_status_url&&(n.get_status_url=t.get_status_url);var i={type:n.request_type,no_login:n.no_login,flag:1,t:(new Date).getTime(),is_ajax:1,resultJSON:1};if(n.order_number&&(i.order_number=n.order_number),!1===n.ident)return!1;n.ident=!1,e.ajax({type:"get",url:n.get_status_url,async:!1,data:i,dataType:"json",timeout:n.timeout_time,crossDomain:!0,xhrFields:{withCredentials:!0},success:function(t){n.getStatusSuccess(t)},error:function(){n.ident=!0}})},getStatusSuccess:function(e){var n=this;1===e.status?(n.ident=!1,n.clearTimer(),n.delScanPayCache(),null!==f?f(e):setTimeout(function(){t.location.replace(e.url)},300)):3===e.status?(0===n.payment_flag&&"function"==typeof n.payment_function&&(n.payment_flag=1,n.payment_function(e)),n.ident=!0):0===e.status?(void 0,n.ident=!1):n.ident=!0},delScanPayCache:function(){(new Image).src=this.cart_url+"mp/mallorder/pay/delScanPayCache?key="+this.orderNo},quickBuy:function(t){var n=this,i=r(t.params);e.ajax({type:"post",url:n.cart_url+"mp/mallorder/order/quickBuy",data:JSON.stringify(i),dataType:"json",contentType:"application/json",xhrFields:{withCredentials:!0},timeout:n.timeout_time,success:function(e){200===e.code?(n.clearTimer(),t.get_pay_success_callback&&t.get_pay_success_callback(e)):400103012===e.code?t.error_function&&t.error_function({status:400103012,errorMessage:"余额不足"}):401==e.code?m():(void 0,t.error_function&&t.error_function({status:15,errorMessage:"提交订单失败"}))},error:function(e){if(401==e.code)return void m();t.error_function&&t.error_function({status:15,errorMessage:"提交订单失败"})},complete:function(t,e){"timeout"===e&&qrPayAjax.abort()}})}},t.csdn=t.csdn||{},t.csdn.cart=new _,t.cart=new _,t.csdn.cartClass=_}(window,jQuery);