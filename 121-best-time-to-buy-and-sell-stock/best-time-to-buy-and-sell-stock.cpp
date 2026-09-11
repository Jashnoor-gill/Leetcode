class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minprice=INT_MAX;
        int maxprice=0;
        for(int price : prices){
            if(price<minprice){
                minprice=price;
            }
            maxprice=max(maxprice,price-minprice);


        }

        return maxprice;    
    }
    
};