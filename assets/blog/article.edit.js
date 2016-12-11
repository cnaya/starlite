$(document).ready(function () {
    var editor = editormd("inputBody", {
        width: "100%",
        height: 800,
        path: '/assets/editor.md/lib/',
        theme: "eclipse",
        previewTheme: "eclipse",
        editorTheme: "eclipse",
        codeFold: true,
        //syncScrolling : false,
        saveHTMLToTextarea: false, // 保存 HTML 到 Textarea
        searchReplace: true,
        //watch : false,                // 关闭实时预览
        //htmlDecode: "style,script,iframe|on*", // 开启 HTML 标签解析，为了安全性，默认不开启    
        //toolbar  : false,             //关闭工具栏
        //previewCodeHighlight : false, // 关闭预览 HTML 的代码块高亮，默认开启
        emoji: true,
        taskList: true,
        tocm: true, // Using [TOCM]
        tex: true, // 开启科学公式TeX语言支持，默认关闭
        flowChart: true, // 开启流程图支持，默认关闭
        sequenceDiagram: true, // 开启时序/序列图支持，默认关闭,
        //dialogLockScreen : false,   // 设置弹出层对话框不锁屏，全局通用，默认为true
        //dialogShowMask : false,     // 设置弹出层对话框显示透明遮罩层，全局通用，默认为true
        //dialogDraggable : false,    // 设置弹出层对话框不可拖动，全局通用，默认为true
        //dialogMaskOpacity : 0.4,    // 设置透明遮罩层的透明度，全局通用，默认值为0.1
        //dialogMaskBgColor : "#000", // 设置透明遮罩层的背景颜色，全局通用，默认为#fff
        imageUpload: true,
        imageFormats: ["jpg", "jpeg", "png"],
        imageUploadURL: "/blog/upload/image",
        onload: function () {
            var $this = this;
            var reg = new RegExp("^/blog/edit/[1-9][0-9]*/?");
            if (reg.test(location.pathname)) {
                var dialog = layer.load();
                var pid = location.pathname.split('/')[3];
                $.post('/blog/detail/' + pid, function (result) {
                    layer.close(dialog);
                    if (result.ok) {
                        $('#inputHead').val(result.head);
                        $('#inputAbstract').val(result.abstract);
                        $('#inputKeywords').val(result.keywords);
                        if (result.publish) {
                            $('input[name=inputPublish]').filter('[value=1]').attr('checked', true);
                        } else {
                            $('input[name=inputPublish]').filter('[value=0]').attr('checked', true);
                        }
                        $this.setMarkdown(result.body);
                        $('#article-submit').attr("pid", pid);
                    } else {
                        layer.alert('没有找到该文章。');
                    }
                });
            }
            //this.fullscreen();
            //this.unwatch();
            //this.watch().fullscreen();

            //this.setMarkdown("#### Markdown Editor");
            //this.width("100%");
            //this.height(480);
            //this.resize("100%", 640);
        }
    });
    $('#article-submit').click(function () {
        var head = $('#inputHead').val(),
                abstract = $('#inputAbstract').val(),
                body = editor.getMarkdown(),
                publish = $('input[name=inputPublish]:checked').val(),
                keywords = $('#inputKeywords').val();

        if (head.length == 0 || abstract.length == 0 || keywords.length == 0 || body.length == 0) {
            layer.alert('表单未填写完整.');
            return;
        }
        if (head.length > 50) {
            layer.alert("标题不能超过50字");
            return;
        }
        if (abstract.length > 200) {
            layer.alert("摘要不能超过200字");
            return;
        }
        if (keywords.length > 80) {
            layer.alert("关键字不能超过20字");
            return;
        }
        var dialog = layer.load();
        var data = {inputHead: head,
            inputAbstract: abstract,
            inputKeywords: keywords.replace(/，|。|；|、|\s+/g, ","),
            inputBody: body,
            inputPublish: publish
        };
        if ($(this).attr("pid") !== undefined) {
            data.pid = $(this).attr("pid");
        }
        $.post('/blog/edit',
                data,
                function (result) {
                    layer.close(dialog);
                    if (result.ok) {
                        layer.alert(result.message);
                        setTimeout(function () {
                            location.href = '/blog/detail/' + result.pid;
                        }, 3000);
                    } else if (typeof (result.login) != 'undefined') {
                        var email, password;
                        layer.prompt({
                            formType: 0,
                            title: '邮箱'
                        }, function (text, index) {
                            if (text.length == 0) {
                                layer.alert('邮箱不能为空。');
                                return;
                            }
                            email = text;
                            layer.close(index);
                            layer.prompt({
                                formType: 1,
                                title: '密码'
                            }, function (text, index) {
                                if (text.length == 0) {
                                    layer.alert("密码不能为空");
                                    return;
                                }
                                password = text;
                                layer.close(index);
                                var dialog = layer.load();
                                $.post("/blog/login", {'inputEmail': email, 'inputPassword': password}, function (result) {
                                    layer.close(dialog)
                                    layer.alert(result.message);
                                    if (result.ok) {
                                        layer.msg('你已经重新登陆。');
                                    }
                                });
                            });
                        });
                    } else {
                        layer.alert(result.message);
                    }
                });
    });

});


