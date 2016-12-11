$(document).ready(function () {
    $('#inputImage').click(function () {
        $('#uploadImage').trigger('click');
    });
    var dialog;
    $('#upload-form input[type="file"]').ajaxfileupload({
        action: '/blog/upload/image',
        valid_extensions: ['png', 'jpeg', 'jpg'],
        onStart: function () {
            layer.msg('开始上传。');
            dialog = layer.load();
        },
        onComplete: function (response) {
            layer.close(dialog);
            if (response.success == 1) {
                $('#inputImage').attr('src', response.url);
            }
            layer.msg(response.message);
        }
    });
    $('#user-edit-submit').click(function (e) {
        e.preventDefault();
        var name = $('#inputName').val(), about = $('#inputAbout').val();
        if (name.length == 0 || about.length == 0) {
            layer.alert('表单未填写完整.');
            return;
        }
        if (name.length > 50) {
            layer.alert("名称不能超过50字");
            return;
        }
        if (about.length > 200) {
            layer.alert("简介不能超过200字");
            return;
        }
        var dialog = layer.load();
        $.post('/user/edit/info', {name: name, about: about}, function (result) {
            layer.close(dialog);
            layer.alert(result.message);
        });
    });
    $('#change-password').click(function (e) {
        e.preventDefault();
        var old_password, new_password;
        layer.prompt({
            formType: 1,
            title: '旧密码'
        }, function (text, index) {
            if (text.length == 0) {
                layer.alert("密码不能为空");
                return;
            }
            old_password = text;
            layer.close(index);
            layer.prompt({formType: 1, title: '新密码'}, function (text, index) {
                if (text.length == 0) {
                    layer.alert("密码不能为空");
                    return;
                }
                new_password = text;
                layer.close(index);
                var dialog = layer.load();
                $.post("/user/edit/password", {oldPassword: old_password, newPassword: new_password}, function (result) {
                    layer.close(dialog)
                    layer.alert(result.message);
                    if (result.ok) {
                        layer.msg('密码已经重新设置。请重新登陆!', function () {
                            location.href = '/blog/logout';
                        });
                    }
                });
            });
        });
    });
});
