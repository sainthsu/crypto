package org.feike.cryptotest;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import org.feike.acrypto.Crypto;

import java.util.ArrayList;

public class MainActivity extends Activity implements AdapterView.OnItemSelectedListener {
    String testData = "sdsdcaevstcdrtwsveaewSEVDtcgr t3wfeqeaesFDXsecwseBRYT7VUGRRYGFEDAEFGSZSEVC5Egr";
    EditText srcEdit,keyEdit;
    private TextView resultText;
    Spinner typeSpinner;
    Button encrypt;
    ArrayList<String> types = new ArrayList<>();
    private ArrayAdapter<String> adapter;
    private int encryptType;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        srcEdit = (EditText) this.findViewById(R.id.src);
        keyEdit = (EditText) this.findViewById(R.id.key);
        resultText = (TextView) this.findViewById(R.id.result);
        typeSpinner = (Spinner) this.findViewById(R.id.type);
        encrypt = (Button) this.findViewById(R.id.encrypt);

        types.add("MD5");
        types.add("AES");
        types.add("BASE64");
        types.add("SHA");

        adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, types);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        //第四步：将适配器添加到下拉列表上
        typeSpinner.setAdapter(adapter);
        typeSpinner.setOnItemSelectedListener(this);

        encrypt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                byte[] data = null;
                String src = srcEdit.getText().toString();
                String key = keyEdit.getText().toString();
                switch (encryptType) {
                    case 1:
                        data = Crypto.MD5(src.getBytes());
                        break;
                    case 2:
                        data = Crypto.AESEncode(src.getBytes(), key.getBytes());
                        break;
                    case 3:
                        data = Crypto.Base64Encode(src.getBytes());
                        break;
                    case 4:
                        data = Crypto.SHA(src.getBytes(), Crypto.SHA1);
                        break;
                    default:
                        data = Crypto.MD5(src.getBytes());
                }

                resultText.setText(new String(data));
            }
        });

    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        encryptType = position + 1;
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
        encryptType = 1;
    }
}
