# Python API Overview [info_overview]

この文書の目的は、どのようにPythonとBlenderがどのように組み合されるかを説明することです。
APIリファレンスとサンプルスクリプトを読んだだけではわからない、いくつかの機能をカバーします。

## Python in Blender

Blenderは組み込みのPythonインタプリタを持っています。
インタプリタはBlender開始時にロードされ、Blender実行中はアクティブになっています。
このインタプリタはユーザーインターフェイスを描画するためのスクリプトを実行し、内部ツールの一部にも使用されています。

Blenderの組み込みインタプリタは、典型的なPython環境を提供します。
そのため、Pythonスクリプトの書き方のチュートリアルにあるコードも、Blenderのインタプリタで実行できます。
Blenderは、 `bpy` や `mathutils` といった
Pythonモジュールを組み込みインタプリタを提供します。
これらのモジュールはスクリプトにインポートされ、Blenderのデータやクラス、関数へのアクセスを提供します。
Blenderのデータを扱うスクリプトは、これらのモジュールをインポートする必要があります。

ここに、"Cube"という名前のオブジェクトの頂点を移動させる簡単なサンプルがあります。

``` python
import bpy
bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
```

このサンプルはBlenderの内部データを直接変更します。
インタラクティブコンソールでこのサンプルを実行すると、3Dビューポートが更新されることを確認できます。

## The Default Environment

自身のスクリプトを開発する際は、BlenderがどのようにPython環境をセットアップするかを理解しておくと役に立つかもしれません。
たくさんのPythonスクリプトがBlenderに付属しており、
ツールを開発する時に使用するAPIと同じものが使われているため、リファレンスとしても利用されます。
スクリプトの典型的な使用法は、ユーザーインターフェイスやインポート/エクスポート、シーン操作、自動化、独自のツールの定義、カスタマイズなどです。

起動時にBlenderは `scripts/startup/`
ディレクトリのPythonモジュールをスキャンし、インポートします。
そのディレクトリの実際の場所は、あなたがインストールした環境に依存します。
`directory layout docs <blender_manual:blender-directory-layout>`
を参照してください。

## Script Loading

これは当たり前のように思えます。しかし、スクリプトを直接実行することと
スクリプトをモジュールとしてインポートすることの違いを知ることは重要です。

スクリプトの実行によりBlenderを拡張するということは、
スクリプトで定義したクラスが、スクリプト実行完了後もBlender内で利用可能であることを意味します。
この方法でスクリプトを使用すると、モジュールとしてスクリプトをインポートする方法と比べて、
将来これらのクラスにアクセスすること(例えば登録解除する)が難しくなります。
スクリプトがモジュールとしてインポートされた時は、そのクラスのインスタンスはモジュールの中に残ります。
そして、あとで、モジュールを再度インポートすることによりアクセスできるようになります。

そのため、クラスを登録してBlenderを拡張するようなスクリプトを直接実行することは避けた方が良いでしょう。

直接Blender内でスクリプトを実行する方法がいくつかあります:

-   テキストエディタにスクリプトをロードし、 *Run Script* ボタンを押す

-   インタラクティブコンソールに入力または貼り付けする

-   コマンドラインでBlenderとともにPythonファイルを実行する。 例:

    ``` sh
    blender --python /home/me/my_script.py
    ```

モジュールとして実行する:

-   簡単な方法は `import some_module`
    コマンドをテキストエディタまたはインタラクティブコンソールで実行する
-   *Register*
    のチェックを付けて、テキストデータブロックとして開きます。これにより、blendファイルといっしょにロードされる
-   起動時に自動的にインポートされる `scripts/startup`
    ディレクトリのいずれにコピーする
-   アドオンとして定義し、アドオンを有効にするとモジュールとしてロードされる

### Add-ons

いくつかのBlenderの機能はオプションのままがベストです。
起動時に読み込まれるスクリプト以外にアドオンがあり、`scripts/addons`
ディレクトリに格納されています。
それらは、ユーザープリファレンスで選択された場合のみ、起動時に読み込まれます。

アドオンとビルトインPythonモジュールとの唯一の違いは、アドオンは
`bl_info` 変数を含む必要があることだけです。
この変数は、名前や作者、カテゴリやプロジェクトのリンクなどのメタデータをBlenderが読むために使われます。
ユーザープリファレンスのアドオンリストは、アドオンの情報を表示するために
`bl_info` を使います。

`bl_info` ディクショナリーの詳細は、 <span class="title-ref">Add-ons
\<https://wiki.blender.org/index.php/Dev:Py/Scripts/Guidelines/Addons>
を参照</span>\_\_

## Integration through Classes

Pythonスクリプトをテキストエディタで実行することは、動作確認には便利ですが、
Blenderを拡張し、他のビルトイン機能のようにアクセスできるツールを作成したいと思うでしょう。

統合が許可されたBlender Python API は以下:

-   `bpy.types.Panel`
-   `bpy.types.Menu`
-   `bpy.types.Operator`
-   `bpy.types.PropertyGroup`
-   `bpy.types.KeyingSet`
-   `bpy.types.RenderEngine`

これは意図して制限されています。現在のところ、
メッシュモディファイアやオブジェクトタイプ、シェーダーノードなど高度な機能を使用する場合は、C/C++を利用する必要があります。

Python統合のために、Blenderは全タイプで共通のメソッドを定義します。
これはBlenderクラスのサブクラスを作成することによって動作します。
BlenderクラスはBlenderとのインターフェイスとしてあらかじめ定義された親クラスで指定された変数や関数を含んでいます。

例:

``` python
import bpy
class SimpleOperator(bpy.types.Operator):
    bl_idname = "object.simple_operator"
    bl_label = "Tool Name"

    def execute(self, context):
        print("Hello World")
        return {'FINISHED'}

bpy.utils.register_class(SimpleOperator)
```

まず、:mod:\`bpy.types\`のメンバーであるクラスのサブクラスを定義します。
これはBelnderに統合できる全てのクラスに共通です。
そして、登録時に、Operator か Panel を区別するために使用されます。

クラスプロパティはどちらも `bl_` プレフィックスではじまっています。
これは規約です。Blenderのプロパティなのか、あなた自身が追加したプロパティか識別するために使われます。
次に、execute関数をみていきましょう。この関数はオペレーターのインスタンスとカレントコンテキストを引数にとります。
共通のプレフィックスは関数には使われません。
最後に、register関数が呼ばれます。これは、クラスを引数にとり、Blenderにクラスをロードします。

参照: [Class Registration](#class-registration)

継承に関して、Blenderは継承に使えるクラスの種類に制限を課しません。
登録チェックには、親クラスで定義された属性や関数が使用されます。

クラスのmix-inの例:

``` python
import bpy
class BaseOperator:
    def execute(self, context):
        print("Hello World BaseClass")
        return {'FINISHED'}

class SimpleOperator(bpy.types.Operator, BaseOperator):
    bl_idname = "object.simple_operator"
    bl_label = "Tool Name"

bpy.utils.register_class(SimpleOperator)
```

これらのクラスが `__init__(self)`
を定義していないことに注意してください。 `__init__()` や `__del__()`
が定義されていれば呼ばれますが、クラスインスタンスのライフタイムは実行時に限られます。
そのため、パネルの例では、再描画される度に新しいインスタンスを持ちます。
このため、パネルインスタンスに変数を保持するケースはほとんどありません。
その代りに、永続的な変数はBlenderのデータに保持されるべきです。
そうすれば、Blenderを再起動したときに状態が復元できます。

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

モーダルオペレーターは例外です。Blender実行時に、それらのインスタンス変数を保持する必要があります。モーダルオペレーターのテンプレートを参照ください。

</div>

一度、クラスをBlenderに登録したら、クラスのインスタンス化や関数の呼び出しはBlenderに任されます。
実際、ほとんどのPython
APIで期待されるように、スクリプトからこれらのクラスのインスタンスを生成できません。
オペレーターを実行させるには、オペレーターAPIを経由して呼び出す必要があります。
例:

``` python
import bpy
bpy.ops.object.simple_operator()
```

ユーザーインターフェイスクラスには、ボタンやウィンドウ、ファイルヘッダやツールバーなど、描画すべきコンテキストが渡されます。
それから、領域が表示される時に描画されます。そのため、Pythonスクリプトから直接呼び出されることはありません。

## Registration [info_overview_registration]

### Module Registration

起動時にロードされるBlenderモジュールは、 関数 `register()` と
`unregister()` が必要です。
これらは、あなたのコードからBlenderが呼び出す *唯一の*
関数です。それ以外は通常のPythonモジュールです。

簡単なBlender Pythonモジュールは、このようになります:

``` python
import bpy

class SimpleOperator(bpy.types.Operator):
    """ See example above """

def register():
    bpy.utils.register_class(SimpleOperator)

def unregister():
    bpy.utils.unregister_class(SimpleOperator)

if __name__ == "__main__":
    register()
```

これらの関数は、通常、クラス登録やメニュー追加を含むスクリプトの一番下に登場します。
自身のツールのデータセットアップなどの内部目的のために、これらの関数を利用することもできます。
しかし、新しいblendファイルがロードされた時に、register
は再実行されないので注意しましょう。

register と unregister
は、Blenderの実行時に、アドオンの切り替えやスクリプトのリロードを可能にするために使用されます。
もし、registerの呼び出しがスクリプトに本体に書かれていたら、インポート時に登録されます。
これは、モジュールのインポートとBlenderへのクラスのロードの間に違いがないことを意味します。
これは、スクリプトが他のモジュールからクラスをインポートする時に問題になります。
どのクラスがいつロードされるかを管理することが難しくなります。

最後の2行はテスト用です:

``` python
if __name__ == "__main__":
    register()
```

これにより、テキストエディタでスクリプトが直接実行し、変更内容をテストできます。
`__main__` は直接実行のために予約されているので、 この `register()`
呼び出しは、スクリプトがモジュールとしてインポートされる時には実行されません。

### Class Registration

Blenderにクラスを登録すると、クラス定義がBlenderに読み込まれます。
そこで、既存の機能と一緒に利用できるようになります。
一度、クラスがロードされたら、 `bpy.types` からアクセスできます。
この時、クラスのオリジナル名ではなく `bl_idname` を使います。

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

これにはいくつか例外があります。クラス名はユニークであることを保証しません。
その場合は、 `bpy.types.Struct.bl_rna_get_subclass_py` を使います。

</div>

クラスをロードするときに、Blenderは整合性チェックを実施、
全ての必要なプロパティと関数が存在するか、プロパティが正しい型か、関数は正しい数の引数を取るかなどを確認します。

ほとんどの場合、気にかける必要はないでしょう。しかし、クラス定義に問題があれば登録時に例外が発生します。

`def execute(self, context, spam)`
のような引数を使用した場合、例外が発生するでしょう:

`ValueError: expected Operator, SimpleOperator class "execute" function to have 2 args, found 3`

`bl_idname = 1` の場合、例外が発生するでしょう:

`TypeError: validating class error: Operator.bl_idname expected a string type, not int`

#### Inter-Class Dependencies

Blenderをカスタマイズする時、自身の設定をいっしょにまとめたいと思うかもしれません。
やはり、他のスクリプトと共存していかなければなりません。
これらのプロパティをグループ化するためには、クラスを定義する必要があります。
グループ内のグループやグループのコレクションに対しては、登録/解除の順番に悩まされることは避けれれません。

カスタムプロパティグループは、それら自身が登録が必要なクラスです。

例えば、もし、マテリアル設定をカスタムエンジンのために保存したい場合:

``` python
# Create new property
# bpy.data.materials[0].my_custom_props.my_float
import bpy

class MyMaterialProps(bpy.types.PropertyGroup):
    my_float: bpy.props.FloatProperty()

def register():
    bpy.utils.register_class(MyMaterialProps)
    bpy.types.Material.my_custom_props: bpy.props.PointerProperty(type=MyMaterialProps)

def unregister():
    del bpy.types.Material.my_custom_props
    bpy.utils.unregister_class(MyMaterialProps)

if __name__ == "__main__":
    register()
```

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

クラスは、プロパティを使用する前に登録される必要があります。そうしなければエラーが発生するでしょう:

`ValueError: bpy_struct "Material" registration error: my_custom_props could not register`

</div>

``` python
# Create new property group with a sub property
# bpy.data.materials[0].my_custom_props.sub_group.my_float
import bpy

class MyMaterialSubProps(bpy.types.PropertyGroup):
    my_float: bpy.props.FloatProperty()

class MyMaterialGroupProps(bpy.types.PropertyGroup):
    sub_group: bpy.props.PointerProperty(type=MyMaterialSubProps)

def register():
    bpy.utils.register_class(MyMaterialSubProps)
    bpy.utils.register_class(MyMaterialGroupProps)
    bpy.types.Material.my_custom_props: bpy.props.PointerProperty(type=MyMaterialGroupProps)

def unregister():
    del bpy.types.Material.my_custom_props
    bpy.utils.unregister_class(MyMaterialGroupProps)
    bpy.utils.unregister_class(MyMaterialSubProps)

if __name__ == "__main__":
    register()
```

<div class="important" markdown="1">

<div class="title" markdown="1">

Important

</div>

:

最下位のクラスが最初に登録される必要があり、解除はその逆です。

</div>

#### Manipulating Classes

プロパティはBlenderの実行中に追加、削除できます。
通常、登録時または解除時に行なわれます。
しかし、いくつかの特殊なケースの場合、スクリプト実行時に型を変更することが便利かもしれません。

例:

``` python
# add a new property to an existing type
bpy.types.Object.my_float: bpy.props.FloatProperty()
# remove
del bpy.types.Object.my_float
```

これは、自身が定義した `PropertyGroup`
サブクラスでも、同様に機能します。

``` python
class MyPropGroup(bpy.types.PropertyGroup):
    pass
MyPropGroup.my_float: bpy.props.FloatProperty()
```

これは以下と同じです:

``` python
class MyPropGroup(bpy.types.PropertyGroup):
    my_float: bpy.props.FloatProperty()
```

#### Dynamic Class Definition (Advanced)

例えば、外部レンダリングエンジンのシェーダー定義など、Blenderに該当データがない場合があります。
そして、それらのデータを型として、その場で定義したり、削除したりすることが便利な場合があります。

``` python
for i in range(10):
    idname = "object.operator_%d" % i

    def func(self, context):
        print("Hello World", self.bl_idname)
        return {'FINISHED'}

    opclass = type("DynOp%d" % i,
                   (bpy.types.Operator, ),
                   {"bl_idname": idname, "bl_label": "Test", "execute": func},
                   )
    bpy.utils.register_class(opclass)
```

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

`type()` は、クラスを定義するために呼び出されます。
これは、Pythonのクラス生成の代替構文です。
クラスを動的に作成するときに適しています。

</div>

To call the operators from the previous example:

> \>\>\> bpy.ops.object.operator_1() Hello World OBJECT_OT_operator_1
> {'FINISHED'}
>
> \>\>\> bpy.ops.object.operator_2() Hello World OBJECT_OT_operator_2
> {'FINISHED'}
